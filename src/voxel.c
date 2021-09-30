#include "stdbool.h"
#include "string.h"

#include "graphic/texture.h"
#include "voxel.h"

voxel_t voxel_new(texid_t faces[6]) {
	voxel_t v;
	v.is_full = true;
	memcpy(&v.faces, faces, sizeof(texid_t) * 6);
	return v;
}

bool voxel_cmp(voxel_t *a, voxel_t *b) {
	if (a->is_full != b->is_full)
		return false;
	if (memcmp(&a->faces, &b->faces, sizeof(texid_t) * 6))
		return false;
	return true;
}