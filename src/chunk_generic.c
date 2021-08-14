#ifndef VXSIZE
#define VXSIZE 8
#endif // VXSIZE

#include "chunk.h"
#include "common.h"

#include <cglm/cglm.h>

#define PASTER(a, b, c) a##b##c
#define EVALUATOR(a, b, c) PASTER(a, b, c)
#define MESH_FUNC(size) EVALUATOR(chunk, size, _greedy)
#define INTX EVALUATOR(uint, VXSIZE, _t)

#define IDX(x, y, z) (((y)*CHUNK_W * CHUNK_D) + ((z)*CHUNK_W) + (x))
#define IS_AIR(v, x, y, z) (!v[IDX(x, y, z)])

#ifndef FACES
#define FACES
// clang-format off
static const voxel_vert_t back_face[6] = {
	{1, 1, 0}, // v0
	{0, 0, 0}, // v2
	{0, 1, 0}, // v1
	{1, 1, 0}, // v0
	{1, 0, 0}, // v3
	{0, 0, 0}, // v2
};
static const voxel_vert_t front_face[6] = {
	{1, 1, 1},
	{0, 1, 1},
	{0, 0, 1},
	{1, 1, 1},
	{0, 0, 1},
	{1, 0, 1},
};
static const voxel_vert_t left_face[6] = {
	{0, 1, 0},
	{0, 0, 1},
	{0, 1, 1},
	{0, 1, 0},
	{0, 0, 0},
	{0, 0, 1},
};
static const voxel_vert_t right_face[6] = {
	{1, 1, 0},
	{1, 1, 1},
	{1, 0, 1},
	{1, 1, 0},
	{1, 0, 1},
	{1, 0, 0},
};
static const voxel_vert_t bottom_face[6] = {
	{0, 0, 1},
	{1, 0, 0},
	{1, 0, 1},
	{0, 0, 1},
	{0, 0, 0},
	{1, 0, 0},
};
static const voxel_vert_t top_face[6] = {
	{0, 1, 1},
	{1, 1, 1},
	{1, 1, 0},
	{0, 1, 1},
	{1, 1, 0},
	{0, 1, 0},
};
// clang-format on
#endif // FACES

void *MESH_FUNC(VXSIZE)(chunk_t *c, u32 *len) {
	INTX *voxels = (INTX*)c->voxels;

	voxel_vert_t *verts = NULL;
	vecm vec = vecm_new((void **)&verts, sizeof(voxel_vert_t));

	for (u32 x = 0; x < CHUNK_W; ++x) {
		for (u32 y = 0; y < CHUNK_H; ++y) {
			for (u32 z = 0; z < CHUNK_D; ++z) {
				if (!IS_AIR(voxels, x, y, z)) {
					u64 s = vec.len;
					vecm_append(&vec, (void **)&verts, back_face, 6);
					vecm_append(&vec, (void **)&verts, front_face, 6);
					vecm_append(&vec, (void **)&verts, left_face, 6);
					vecm_append(&vec, (void **)&verts, right_face, 6);
					vecm_append(&vec, (void **)&verts, bottom_face, 6);
					vecm_append(&vec, (void **)&verts, top_face, 6);
					for (u64 i = s; i < s + 36; ++i) {
						verts[i].x += x;
						verts[i].y += y;
						verts[i].z += z;
					}
				}
			}
		}
	}


	*len = vec.len;
	return verts;
}

#undef IS_AIR
#undef VXSIZE
#undef EVALUATOR
#undef PASTER
#undef MESH_FUNC
