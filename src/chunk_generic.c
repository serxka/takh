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
#define NORM_BACK 0
#define NORM_FRONT 1
#define NORM_LEFT 2
#define NORM_RIGHT 3
#define NORM_BOTTOM 4
#define NORM_TOP 5
static const voxel_vert_t back_face[6] = {
	{1, 1, 0, 0, 1.f, 1.f, 0}, // v0
	{0, 1, 0, 0, 0.f, 1.f, 0}, // v1
	{0, 0, 0, 0, 0.f, 0.f, 0}, // v2
	{1, 1, 0, 0, 1.f, 1.f, 0}, // v0
	{0, 0, 0, 0, 0.f, 0.f, 0}, // v2
	{1, 0, 0, 0, 1.f, 0.f, 0}, // v3
};
static const voxel_vert_t front_face[6] = {
	{1, 1, 1, 1, 1.f, 1.f, 0},
	{0, 0, 1, 1, 0.f, 0.f, 0},
	{0, 1, 1, 1, 0.f, 1.f, 0},
	{1, 1, 1, 1, 1.f, 1.f, 0},
	{1, 0, 1, 1, 1.f, 0.f, 0},
	{0, 0, 1, 1, 0.f, 0.f, 0},
};
static const voxel_vert_t left_face[6] = {
	{0, 1, 0, 2, 1.f, 0.f, 0},
	{0, 1, 1, 2, 1.f, 1.f, 0},
	{0, 0, 1, 2, 0.f, 1.f, 0},
	{0, 1, 0, 2, 1.f, 0.f, 0},
	{0, 0, 1, 2, 0.f, 1.f, 0},
	{0, 0, 0, 2, 0.f, 0.f, 0},
};
static const voxel_vert_t right_face[6] = {
	{1, 1, 0, 3, 1.f, 0.f, 0},
	{1, 0, 1, 3, 0.f, 1.f, 0},
	{1, 1, 1, 3, 1.f, 1.f, 0},
	{1, 1, 0, 3, 1.f, 0.f, 0},
	{1, 0, 0, 3, 0.f, 0.f, 0},
	{1, 0, 1, 3, 0.f, 1.f, 0},
};
static const voxel_vert_t bottom_face[6] = {
	{0, 0, 1, 4, 0.f, 1.f, 0},
	{1, 0, 1, 4, 1.f, 1.f, 0},
	{1, 0, 0, 4, 1.f, 0.f, 0},
	{0, 0, 1, 4, 0.f, 1.f, 0},
	{1, 0, 0, 4, 1.f, 0.f, 0},
	{0, 0, 0, 4, 0.f, 0.f, 0},
};
static const voxel_vert_t top_face[6] = {
	{0, 1, 1, 5, 0.f, 1.f, 0},
	{1, 1, 0, 5, 1.f, 0.f, 0},
	{1, 1, 1, 5, 1.f, 1.f, 0},
	{0, 1, 1, 5, 0.f, 1.f, 0},
	{0, 1, 0, 5, 0.f, 0.f, 0},
	{1, 1, 0, 5, 1.f, 0.f, 0},
};
// clang-format on
#endif // FACES

void *MESH_FUNC(VXSIZE)(chunk_t *c, u32 *len) {
	INTX *voxels = (INTX *)c->voxels;

	voxel_vert_t *verts = NULL;
	vecm vec = vecm_new((void **)&verts, sizeof(voxel_vert_t));

	for (u32 x = 0; x < CHUNK_W; ++x) {
		for (u32 y = 0; y < CHUNK_H; ++y) {
			for (u32 z = 0; z < CHUNK_D; ++z) {
				INTX vi = voxels[IDX(x, y, z)];
				if (vi != 0) {
					voxel_t *v = c->palette.voxels + vi;
					// Back face
					if (z == 0
					    || IS_AIR(voxels, x, y, z - 1)) {
						u32 s = vec.len;
						vecm_append(&vec,
						            (void **)&verts,
						            back_face,
						            ARR_LEN(back_face));
						for (u32 i = s; i < s + 6;
						     ++i) {
							verts[i].x += x;
							verts[i].y += y;
							verts[i].z += z;
							verts[i].ti =
							        v->faces[i - s];
						}
					}
					// Front face
					if (z == CHUNK_D - 1
					    || IS_AIR(voxels, x, y, z + 1)) {
						u32 s = vec.len;
						vecm_append(
						        &vec, (void **)&verts,
						        front_face,
						        ARR_LEN(front_face));
						for (u32 i = s; i < s + 6;
						     ++i) {
							verts[i].x += x;
							verts[i].y += y;
							verts[i].z += z;
							verts[i].ti =
							        v->faces[i - s];
						}
					}
					// Left face
					if (x == 0
					    || IS_AIR(voxels, x - 1, y, z)) {
						u32 s = vec.len;
						vecm_append(&vec,
						            (void **)&verts,
						            left_face,
						            ARR_LEN(left_face));
						for (u32 i = s; i < s + 6;
						     ++i) {
							verts[i].x += x;
							verts[i].y += y;
							verts[i].z += z;
							verts[i].ti =
							        v->faces[i - s];
						}
					}
					// Right face
					if (x == CHUNK_W - 1
					    || IS_AIR(voxels, x + 1, y, z)) {
						u32 s = vec.len;
						vecm_append(
						        &vec, (void **)&verts,
						        right_face,
						        ARR_LEN(right_face));
						for (u32 i = s; i < s + 6;
						     ++i) {
							verts[i].x += x;
							verts[i].y += y;
							verts[i].z += z;
							verts[i].ti =
							        v->faces[i - s];
						}
					}
					// Bottom face
					if (y == 0
					    || IS_AIR(voxels, x, y - 1, z)) {
						u32 s = vec.len;
						vecm_append(
						        &vec, (void **)&verts,
						        bottom_face,
						        ARR_LEN(bottom_face));
						for (u32 i = s; i < s + 6;
						     ++i) {
							verts[i].x += x;
							verts[i].y += y;
							verts[i].z += z;
							verts[i].ti =
							        v->faces[i - s];
						}
					}
					// Top face
					if (y == CHUNK_H - 1
					    || IS_AIR(voxels, x, y + 1, z)) {
						u32 s = vec.len;
						vecm_append(&vec,
						            (void **)&verts,
						            top_face,
						            ARR_LEN(top_face));
						for (u32 i = s; i < s + 6;
						     ++i) {
							verts[i].x += x;
							verts[i].y += y;
							verts[i].z += z;
							verts[i].ti =
							        v->faces[i - s];
						}
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
