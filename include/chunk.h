#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <stddef.h>

#include "common.h"
#include "graphic/vertex.h"
#include "voxel.h"

#define CHUNK_W 64
#define CHUNK_D 64
#define CHUNK_H 64
#define CHUNK_LEN (CHUNK_W * CHUNK_D * CHUNK_H)

enum PalleteType { PALETTE_EMPTY = 0, PALETTE_SMALL, PALETTE_LARGE };

typedef struct palette {
	enum PalleteType type;
	struct {
		voxel_t *voxels;
		size_t len;
		size_t cap;
		size_t last_free;
	};
} palette_t;

palette_t palette_new(void);
u16 palette_add(palette_t *p, voxel_t *v);
u16 palette_find(palette_t *p, voxel_t *v);
void palette_remove(palette_t *p, u16 index);
void palette_delete(palette_t *p);

typedef struct chunk {
	palette_t palette;
	void *voxels;
} chunk_t;

chunk_t chunk_new(void);
void chunk_alloc(chunk_t *c);
void chunk_set(chunk_t *c, u32 x, u32 y, u32 z, u16 id);
void chunk_setv(chunk_t *c, u32 x, u32 y, u32 z, voxel_t *v);
voxel_t *chunk_get(chunk_t *c, u32 x, u32 y, u32 z);
verts_t chunk_mesh(chunk_t *c);
void chunk_delete(chunk_t *c);

#endif // _CHUNK_H_
