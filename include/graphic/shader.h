#ifndef _GRAPHIC_SHADER_H_
#define _GRAPHIC_SHADER_H_

#include "graphic/gl.h"

#include <cglm/cglm.h>

typedef struct shader {
	GLuint id;
} shader_t;

#define INVALID_SHADER ((shader_t){0})

shader_t shader_compile(const char *vs, const char *fs);
void shader_bind(shader_t *s);
void set_uni_int(shader_t *s, int loc, int val);
void set_uni_vec3(shader_t *s, int loc, vec3 vec);
void set_uni_vec3i(shader_t *s, int loc, int v0, int v1, int v2);
void set_uni_mat4(shader_t *s, int loc, mat4 mat);
void shader_delete(shader_t *s);

#endif // _GRAPHIC_SHADER_H_
