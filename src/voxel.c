#include "voxel.h"

bool voxel_cmp(voxel_t *a, voxel_t *b) {
	if (a->is_full != b->is_full)
		return false;
	return true;
}