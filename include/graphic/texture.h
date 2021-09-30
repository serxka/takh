#ifndef _GRAPHIC_TEXTURE_H_
#define _GRAPHIC_TEXTURE_H_

#include "common.h"
#include "graphic/gl.h"

typedef u32 texid_t;

typedef struct texarr {
	GLuint id;
	u32 depth;
} texarr_t;

texarr_t texarr_new(const char *path, u32 depth);
void texarr_bind(texarr_t *t, u32 slot);
void texarr_delete(texarr_t *t);

#endif // _GRAPHIC_TEXTURE_H_