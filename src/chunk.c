#include <string.h>
#include <stdio.h>

#include "chunk.h"
#include "common.h"

palette_t palette_new(void) {
	palette_t pal = {0};
	pal.last_free = 1;
	return pal;
}

static void grow_palette(palette_t *p, size_t amt) {
	size_t new_cap = p->cap + amt;
	voxel_t *mem = (voxel_t *)realloc(p->voxels, new_cap * sizeof(voxel_t));
	if (mem == NULL)
		fatalln("palette realloc fail");
	p->cap = new_cap;
	memset(mem + p->len, 0, (p->cap - p->len) * sizeof(voxel_t));
	p->voxels = mem;
}

u16 palette_add(palette_t *p, voxel_t *v) {
	// Check we haven't added to many voxels
	if (p->len == UINT16_MAX)
		fatalln("tried to increase chunk palette size above UINT16_MAX");
	// See if we are trying to add air
	if (voxel_cmp(v, &VOXEL_AIR))
		return 0;
	// If this is the first voxel promote our palette
	if (p->len == 0) {
		p->len = 1;
		p->type = PALETTE_SMALL;
		grow_palette(p, 16);
		p->voxels[0] = VOXEL_AIR;
	}
	// See if last free is empty
	if (p->last_free && voxel_cmp(&VOXEL_AIR, p->voxels + p->last_free)) {
		p->voxels[p->last_free] = *v;
		size_t last = p->last_free;
		p->last_free = 0;
		++p->len;
		return last;
	}
	u64 hole = 0;
	// Look for a hole
	for (u64 i = 1; i < p->len; ++i) {
		if (voxel_cmp(p->voxels + i, &VOXEL_AIR)) {
			hole = i;
			break;
		}
	}
	// If did not find a hole chuck it at the end
	if (hole == 0) {
		hole = p->len;
		// Grow if we need more space
		if (p->len == p->cap)
			grow_palette(p, 16);
	}
	p->voxels[hole] = *v;
	++p->len;
	// Check if we need to update the type of our palette
	if (p->len >= UINT8_MAX)
		p->type = PALETTE_LARGE;

	return hole;
}

u16 palette_find(palette_t *p, voxel_t *v) {
	for (u64 i = 0; i < p->len; ++i) {
		if (voxel_cmp(v, p->voxels + i))
			return i;
	}
	return 0;
}

void palette_remove(palette_t *p, u16 index) {
	if (index >= p->len)
		return;
	p->voxels[index] = VOXEL_AIR;
	p->last_free = index;
}

void palette_delete(palette_t *p) {
	if (p->voxels != NULL)
		free(p->voxels);
	*p = (palette_t){0};
}


#define IDX(x, y, z) (((y)*CHUNK_W * CHUNK_D) + ((z)*CHUNK_W) + (x))

#define VXSIZE 8
#include "chunk_generic.c"
#define VXSIZE 16
#include "chunk_generic.c"

chunk_t chunk_new(void) {
	chunk_t c;
	c.palette = palette_new();
	c.voxels = NULL;
	return c;
}

static void promote_small(chunk_t *c) {
	assert(c->palette.type != PALETTE_EMPTY);
	c->voxels = alloc(CHUNK_LEN, sizeof(u8));
}

static void promote_large(chunk_t *c) {
	assert(c->palette.type != PALETTE_SMALL);
	u16 *new = (u16 *)alloc(CHUNK_LEN, sizeof(u16));
	u8 *old = (u8 *)c->voxels;
	for (u64 i = 0; i < CHUNK_LEN; ++i)
		new[i] = old[i];
	free(c->voxels);
	c->voxels = (void *)new;
}

void chunk_alloc(chunk_t *c) {
	if (c->palette.type == PALETTE_EMPTY)
		return;
	if (c->palette.type == PALETTE_SMALL && c->voxels == NULL) 
		promote_small(c);
	else if (c->palette.type == PALETTE_LARGE && c->voxels == NULL)
		c->voxels = alloc(CHUNK_LEN, sizeof(u16));
}

void chunk_set(chunk_t *c, u32 x, u32 y, u32 z, u16 id) {
	switch (c->palette.type) {
	case PALETTE_EMPTY:
		// no-op
		return;
	case PALETTE_SMALL:
		((u8 *)c->voxels)[IDX(x, y, z)] = id;
		return;
	case PALETTE_LARGE:
		((u16 *)c->voxels)[IDX(x, y, z)] = id;
		return;
	}
}

void chunk_setv(chunk_t *c, u32 x, u32 y, u32 z, voxel_t *v) {
	// Add to the palette
	palette_t *pal = &c->palette;
	enum PalleteType type = pal->type;
	u16 id = palette_find(pal, v);
	if (id == 0)
		id = palette_add(pal, v);
	// Check if we need to resize
	if (type == PALETTE_SMALL && pal->type == PALETTE_LARGE)
		promote_large(c);
	else if (type == PALETTE_EMPTY && pal->type == PALETTE_SMALL)
		promote_small(c);
	// Set the voxel
	chunk_set(c, x, y, z, id);
}

voxel_t *chunk_get(chunk_t *c, u32 x, u32 y, u32 z) {
	switch (c->palette.type) {
	case PALETTE_SMALL: {
		u8 *voxels = (u8 *)c->voxels;
		return c->palette.voxels + voxels[IDX(x, y, z)];
	}
	case PALETTE_LARGE: {
		u16 *voxels = (u16 *)c->voxels;
		return c->palette.voxels + voxels[IDX(x, y, z)];
	}
	default:
		return &VOXEL_AIR;
	}
}

verts_t chunk_mesh(chunk_t *c) {
	void *verts;
	u32 len;
	switch (c->palette.type) {
	case PALETTE_EMPTY:
		verts = NULL;
		len = 0;
		break;
	case PALETTE_SMALL:
		verts = chunk8_greedy(c, &len);
		break;
	case PALETTE_LARGE:
		verts = chunk16_greedy(c, &len);
		break;
	}
	return verts_create(verts, len, voxel_attrib);
}

void chunk_delete(chunk_t *c) {
	if (c->voxels != NULL)
		free(c->voxels);
	palette_delete(&c->palette);
	*c = (chunk_t){0};
}
