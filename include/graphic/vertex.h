#ifndef _GRAPHIC_VERTEX_H_
#define _GRAPHIC_VERTEX_H_

#include "common.h"
#include "graphic/gl.h"

typedef struct verts {
	GLuint vao;
	GLuint vbo;
	u32 count;
	u32 count_cap;
	u8 size;
} verts_t;

enum AttributeType {
	ATTRIB_END = 0,
	ATTRIB_BYTE1 = 1,
	ATTRIB_BYTE3,
	ATTRIB_UINT1,
	ATTRIB_FLOAT2,
	ATTRIB_FLOAT3,
};

verts_t verts_create(const void *data, u32 count,
                     const enum AttributeType *types);
void verts_update(verts_t *v, u8 *data, u32 count);
void verts_draw(verts_t *v);
void verts_delete(verts_t *v);

#endif // _GRAPHIC_VERTEX_H_
