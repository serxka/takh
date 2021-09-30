#include "graphic/texture.h"
#include "common.h"
#include "graphic/gl.h"
#include "resource.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

texarr_t texarr_new(const char *path, u32 depth) {
	u64 imglen;
	u8 *mem = (u8 *)res_request(path, &imglen);
	if (mem == NULL)
		goto fail_load;
	int w, h, c;
	mem = stbi_load_from_memory(mem, imglen, &w, &h, &c, 0);
	if (mem == NULL)
		goto fail_load;

	u32 width = w;
	u32 height = h / depth;

	GLenum format, internal;
	switch (c) {
	case 1:
		format = GL_RED;
		internal = GL_R8;
		break;
	case 3:
		format = GL_RGB;
		internal = GL_RGB8;
		break;
	case 4:
		format = GL_RGBA;
		internal = GL_RGBA8;
		break;
	default:
		goto fail_load;
	}

	GLuint id;
	gl_CreateTextures(GL_TEXTURE_2D_ARRAY, 1, &id);
	gl_TextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gl_TextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	gl_TextureStorage3D(id, 1, internal, width, height, depth);
	for (u64 i = 0; i < depth; ++i) {
		void *img = mem + (width * height * c * i);
		gl_TextureSubImage3D(id, 0, 0, 0, i, width, height, 1, format,
		                     GL_UNSIGNED_BYTE, img);
	}
	return (texarr_t){.id = id, .depth = depth};
fail_load:
	errorln("Failed to load texture array, image: '%s'", path);
	return (texarr_t){0};
}

void texarr_bind(texarr_t *t, u32 slot) {
	gl_BindTextureUnit(slot, t->id);
}

void texarr_delete(texarr_t *t) {
	gl_DeleteTextures(1, &t->id);
}
