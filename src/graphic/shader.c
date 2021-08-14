#include <stddef.h>

#include "common.h"
#include "graphic/gl.h"
#include "graphic/shader.h"
#include "resource.h"

#include <cglm/cglm.h>

shader_t shader_compile(const char *vs_path, const char *fs_path) {
	// Get our shader source codes
	u64 vs_len, fs_len;
	char *vs_src = (char *)res_request(vs_path, &vs_len);
	char *fs_src = (char *)res_request(fs_path, &fs_len);
	if (vs_src == NULL || fs_src == NULL)
		return INVALID_SHADER;

	GLint res;
	char log[2048];

	// Create our vertex shader
	GLuint vs = gl_CreateShader(GL_VERTEX_SHADER);
	gl_ShaderSource(vs, 1, &vs_src, (s32 *)&vs_len);
	gl_CompileShader(vs);
	// Check for compile errors
	gl_GetShaderiv(vs, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		gl_GetShaderInfoLog(vs, ARR_LEN(log), NULL, log);
		errorln("error compiling shader '%s': %s", vs_path, log);
		return INVALID_SHADER;
	}

	// Create our fragment shader
	GLuint fs = gl_CreateShader(GL_FRAGMENT_SHADER);
	gl_ShaderSource(fs, 1, &fs_src, (s32 *)&fs_len);
	gl_CompileShader(fs);
	// Check for compile errors
	gl_GetShaderiv(fs, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		gl_GetShaderInfoLog(fs, ARR_LEN(log), NULL, log);
		errorln("error compiling shader '%s': %s", fs_path, log);
		return INVALID_SHADER;
	}

	// Create our shader program
	GLint id = gl_CreateProgram();
	gl_AttachShader(id, vs);
	gl_AttachShader(id, fs);
	gl_LinkProgram(id);
	// Check for link errors
	gl_GetProgramiv(id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE) {
		gl_GetProgramInfoLog(id, ARR_LEN(log), NULL, log);
		errorln("error linking shader ('%s', '%s') (%d): %s", vs_path,
		        fs_path, res, log);
		return INVALID_SHADER;
	}

	// Clean up resources
	gl_DetachShader(id, vs);
	gl_DeleteShader(vs);
	gl_DetachShader(id, fs);
	gl_DeleteShader(fs);

	return (shader_t){id};
}

void shader_bind(shader_t *s) {
	gl_UseProgram(s->id);
}

void set_uni_int(shader_t *s, int loc, int val) {
	gl_ProgramUniform1i(s->id, loc, val);
}

void set_uni_vec3(shader_t *s, int loc, vec3 vec) {
	gl_ProgramUniform3f(s->id, loc, vec[0], vec[1], vec[2]);
}

void set_uni_vec3i(shader_t *s, int loc, int v0, int v1, int v2) {
	gl_ProgramUniform3i(s->id, loc, v0, v1, v2);
}

void set_uni_mat4(shader_t *s, int loc, mat4 mat) {
	gl_ProgramUniformMatrix4fv(s->id, loc, 1, GL_FALSE, (float *)mat);
}

void shader_delete(shader_t *s) {
	gl_DeleteProgram(s->id);
}
