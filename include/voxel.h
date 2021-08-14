#ifndef _VOXEL_H_
#define _VOXEL_H_

#include <stdbool.h>

#include "common.h"
#include "graphic/vertex.h"

typedef struct voxel {
	bool is_full;
} voxel_t;

#define VOXEL_AIR ((voxel_t){0})

bool voxel_cmp(voxel_t *a, voxel_t *b);

static const enum AttributeType voxel_attrib[] = {ATTRIB_BYTE3, ATTRIB_END};
typedef struct PACKED voxel_vert {
	u8 x, y, z;
} voxel_vert_t;

#endif // _VOXEL_H_
