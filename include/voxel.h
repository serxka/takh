#ifndef _VOXEL_H_
#define _VOXEL_H_

#include <stdbool.h>

#include "common.h"
#include "graphic/texture.h"
#include "graphic/vertex.h"

enum VoxelFace {
	FACE_BACK = 0,
	FACE_FRONT = 1,
	FACE_LEFT = 2,
	FACE_RIGHT = 3,
	FACE_BOTTOM = 4,
	FACE_TOP = 5,
};

typedef struct voxel {
	bool is_full;
	texid_t faces[6];
} voxel_t;

#define VOXEL_AIR ((voxel_t){0})

voxel_t voxel_new(texid_t faces[6]);
bool voxel_cmp(voxel_t *a, voxel_t *b);

static const enum AttributeType voxel_attrib[] = {
        ATTRIB_BYTE3, ATTRIB_BYTE1, ATTRIB_FLOAT2, ATTRIB_UINT1, ATTRIB_END};
typedef struct PACKED voxel_vert {
	u8 x, y, z;
	u8 norm;
	float u, v;
	u32 ti;
} voxel_vert_t;

#endif // _VOXEL_H_
