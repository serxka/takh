#include "graphic/vertex.h"
#include "common.h"
#include "graphic/gl.h"

verts_t verts_create(const void *data, u32 count,
                     const enum AttributeType *types) {
	// Create a vertex array object and vertex buffer
	GLuint vao, vbo;
	gl_CreateVertexArrays(1, &vao);
	gl_CreateBuffers(1, &vbo);

	// Calculate the size of our vertices based on our attributes
	u8 vert_size = 0;
	const enum AttributeType *ty = types;
	while (*ty)
		switch (*ty++) {
		case ATTRIB_BYTE1:
			vert_size += 1;
			break;
		case ATTRIB_BYTE3:
			vert_size += 3;
			break;
		case ATTRIB_UINT1:
			vert_size += 4;
			break;
		case ATTRIB_FLOAT2:
			vert_size += 8;
			break;
		case ATTRIB_FLOAT3:
			vert_size += 12;
			break;
		case ATTRIB_END:
			break;
		}

	// Attach our VBO to our VAO for when we bind it
	gl_VertexArrayVertexBuffer(vao, 0, vbo, 0, vert_size);

	// Loop through and set our attributes
	u32 offset = 0;
	u32 index = 0;
	while (*types) {
		// Enable our vertex attribute on index
		gl_EnableVertexArrayAttrib(vao, index);
		switch (*types++) {
		case ATTRIB_BYTE1:
			// Set the binding onto our vao
			gl_VertexArrayAttribBinding(vao, index, 0);
			// The attribute format
			gl_VertexArrayAttribIFormat(vao, index, 1,
			                            GL_UNSIGNED_BYTE, offset);
			offset += 1;
			break;
		case ATTRIB_BYTE3:
			gl_VertexArrayAttribBinding(vao, index, 0);
			gl_VertexArrayAttribFormat(vao, index, 3,
			                           GL_UNSIGNED_BYTE, GL_FALSE,
			                           offset);
			offset += 3;
			break;
		case ATTRIB_UINT1:
			gl_VertexArrayAttribBinding(vao, index, 0);
			gl_VertexArrayAttribIFormat(vao, index, 1,
			                            GL_UNSIGNED_INT, offset);
			offset += 4;
			break;
		case ATTRIB_FLOAT2:
			gl_VertexArrayAttribBinding(vao, index, 0);
			gl_VertexArrayAttribFormat(vao, index, 2, GL_FLOAT,
			                           GL_FALSE, offset);
			offset += 8;
			break;
		case ATTRIB_FLOAT3:
			gl_VertexArrayAttribBinding(vao, index, 0);
			gl_VertexArrayAttribFormat(vao, index, 3, GL_FLOAT,
			                           GL_FALSE, offset);
			offset += 12;
			break;
		case ATTRIB_END:
			break;
		}
		++index;
	}

	if (count)
		gl_NamedBufferData(vbo, count * vert_size, data,
		                   GL_STATIC_DRAW);

	return (verts_t){
	        .vao = vao,
	        .vbo = vbo,
	        .count = count,
	        .count_cap = count,
	        .size = vert_size,
	};
}

void verts_update(verts_t *v, u8 *data, u32 count) {
	// Set the new count
	v->count = count;
	// If it is zero don't bother changing the buffer
	if (!v->count)
		return;
	if (v->count > v->count_cap) { // Reallocate the buffer
		gl_NamedBufferData(v->vbo, v->count * v->size, data,
		                   GL_STATIC_DRAW);
		v->count_cap = v->count;
	} else { // Reuse the buffer
		gl_NamedBufferSubData(v->vbo, 0, v->count * v->size, data);
	}
}

void verts_draw(verts_t *v) {
	if (v->count) {
		gl_BindVertexArray(v->vao);
		gl_DrawArrays(GL_TRIANGLES, 0, v->count);
	}
}

void verts_delete(verts_t *v) {
	gl_DeleteVertexArrays(1, &v->vao);
	gl_DeleteBuffers(1, &v->vbo);
}
