// DO NOT EDIT THIS FILE BY HAND

#include <GLFW/glfw3.h>

#include "common.h"
#include "graphic/gl.h"

#define LOADPROC(func)                                                         \
	do {                                                                   \
		void (**fn)(void) = (void (**)(void)) & gl_##func;             \
		*fn = glfwGetProcAddress("gl" #func);                          \
		if (*fn == (void *)0)                                          \
			fatalln("failed to load symbol gl" #func);             \
	} while (0)

GLuint (*gl_CreateProgram)(void);
GLuint (*gl_CreateShader)(GLenum shaderType);
void (*gl_AttachShader)(GLuint program, GLuint shader);
void (*gl_BindVertexArray)(GLuint array);
void (*gl_Clear)(GLbitfield mask);
void (*gl_ClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (*gl_CompileShader)(GLuint shader);
void (*gl_CreateBuffers)(GLsizei n, GLuint *buffers);
void (*gl_CreateVertexArrays)(GLsizei n, GLuint *arrays);
void (*gl_CullFace)(GLenum mode);
void (*gl_DebugMessageCallback)(void (*callback)(GLenum source, GLenum type,
                                                 GLuint id, GLenum severity,
                                                 GLsizei length,
                                                 const GLchar *message,
                                                 const void *userParam),
                                const void *userParam);
void (*gl_DeleteBuffers)(GLsizei n, const GLuint *buffers);
void (*gl_DeleteProgram)(GLuint program);
void (*gl_DeleteShader)(GLuint shader);
void (*gl_DeleteVertexArrays)(GLsizei n, const GLuint *arrays);
void (*gl_DetachShader)(GLuint program, GLuint shader);
void (*gl_DrawArrays)(GLenum mode, GLint first, GLsizei count);
void (*gl_Enable)(GLenum cap);
void (*gl_EnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
void (*gl_FrontFace)(GLenum mode);
void (*gl_GetIntegerv)(GLenum pname, GLint *data);
void (*gl_GetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei *length,
                             GLchar *logInfo);
void (*gl_GetProgramiv)(GLuint program, GLenum pname, GLint *params);
void (*gl_GetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei *length,
                            GLchar *logInfo);
void (*gl_GetShaderiv)(GLuint shader, GLenum pname, GLint *params);
void (*gl_LinkProgram)(GLuint program);
void (*gl_NamedBufferData)(GLuint buffer, GLsizeiptr size, const void *data,
                           GLenum usage);
void (*gl_NamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size,
                              const void *data);
void (*gl_ProgramUniform1i)(GLuint program, GLint location, GLint value);
void (*gl_ProgramUniform3f)(GLuint program, GLint location, GLfloat v0,
                            GLfloat v1, GLfloat v2);
void (*gl_ProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1,
                            GLint v2);
void (*gl_ProgramUniformMatrix4fv)(GLuint program, GLint location,
                                   GLsizei count, GLboolean transpose,
                                   const GLfloat *value);
void (*gl_ShaderSource)(GLuint shader, GLsizei count, GLchar **string,
                        GLint *length);
void (*gl_UseProgram)(GLuint program);
void (*gl_VertexArrayAttribBinding)(GLuint vaobj, GLuint attribindex,
                                    GLuint bindingindex);
void (*gl_VertexArrayAttribFormat)(GLuint vaobj, GLuint attribindex, GLint size,
                                   GLenum type, GLboolean normalized,
                                   GLuint relativeoffset);
void (*gl_VertexArrayVertexBuffer)(GLuint vaobj, GLuint bindingindex,
                                   GLuint buffer, GLintptr offset,
                                   GLsizei stride);
void (*gl_Viewport)(GLint x, GLint y, GLsizei width, GLsizei height);

void gl_loadproc(void) {
	LOADPROC(CreateProgram);
	LOADPROC(CreateShader);
	LOADPROC(AttachShader);
	LOADPROC(BindVertexArray);
	LOADPROC(Clear);
	LOADPROC(ClearColor);
	LOADPROC(CompileShader);
	LOADPROC(CreateBuffers);
	LOADPROC(CreateVertexArrays);
	LOADPROC(CullFace);
	LOADPROC(DebugMessageCallback);
	LOADPROC(DeleteBuffers);
	LOADPROC(DeleteProgram);
	LOADPROC(DeleteShader);
	LOADPROC(DeleteVertexArrays);
	LOADPROC(DetachShader);
	LOADPROC(DrawArrays);
	LOADPROC(Enable);
	LOADPROC(EnableVertexArrayAttrib);
	LOADPROC(FrontFace);
	LOADPROC(GetIntegerv);
	LOADPROC(GetProgramInfoLog);
	LOADPROC(GetProgramiv);
	LOADPROC(GetShaderInfoLog);
	LOADPROC(GetShaderiv);
	LOADPROC(LinkProgram);
	LOADPROC(NamedBufferData);
	LOADPROC(NamedBufferSubData);
	LOADPROC(ProgramUniform1i);
	LOADPROC(ProgramUniform3f);
	LOADPROC(ProgramUniform3i);
	LOADPROC(ProgramUniformMatrix4fv);
	LOADPROC(ShaderSource);
	LOADPROC(UseProgram);
	LOADPROC(VertexArrayAttribBinding);
	LOADPROC(VertexArrayAttribFormat);
	LOADPROC(VertexArrayVertexBuffer);
	LOADPROC(Viewport);
}

void gl_loadproc_minimal(void) {
	LOADPROC(GetIntegerv);
}