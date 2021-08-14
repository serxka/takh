// DO NOT EDIT THIS FILE BY HAND

#ifndef _GL_H_
#define _GL_H_

#include <GLFW/glfw3.h>

extern GLuint (*gl_CreateProgram)(void);
extern GLuint (*gl_CreateShader)(GLenum shaderType);
extern void (*gl_AttachShader)(GLuint program, GLuint shader);
extern void (*gl_BindVertexArray)(GLuint array);
extern void (*gl_Clear)(GLbitfield mask);
extern void (*gl_ClearColor)(GLfloat red, GLfloat green, GLfloat blue,
                             GLfloat alpha);
extern void (*gl_CompileShader)(GLuint shader);
extern void (*gl_CreateBuffers)(GLsizei n, GLuint *buffers);
extern void (*gl_CreateVertexArrays)(GLsizei n, GLuint *arrays);
extern void (*gl_CullFace)(GLenum mode);
extern void (*gl_DebugMessageCallback)(
        void (*callback)(GLenum source, GLenum type, GLuint id, GLenum severity,
                         GLsizei length, const GLchar *message,
                         const void *userParam),
        const void *userParam);
extern void (*gl_DeleteBuffers)(GLsizei n, const GLuint *buffers);
extern void (*gl_DeleteProgram)(GLuint program);
extern void (*gl_DeleteShader)(GLuint shader);
extern void (*gl_DeleteVertexArrays)(GLsizei n, const GLuint *arrays);
extern void (*gl_DetachShader)(GLuint program, GLuint shader);
extern void (*gl_DrawArrays)(GLenum mode, GLint first, GLsizei count);
extern void (*gl_Enable)(GLenum cap);
extern void (*gl_EnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
extern void (*gl_FrontFace)(GLenum mode);
extern void (*gl_GetIntegerv)(GLenum pname, GLint *data);
extern void (*gl_GetProgramInfoLog)(GLuint program, GLsizei maxLength,
                                    GLsizei *length, GLchar *logInfo);
extern void (*gl_GetProgramiv)(GLuint program, GLenum pname, GLint *params);
extern void (*gl_GetShaderInfoLog)(GLuint shader, GLsizei maxLength,
                                   GLsizei *length, GLchar *logInfo);
extern void (*gl_GetShaderiv)(GLuint shader, GLenum pname, GLint *params);
extern void (*gl_LinkProgram)(GLuint program);
extern void (*gl_NamedBufferData)(GLuint buffer, GLsizeiptr size,
                                  const void *data, GLenum usage);
extern void (*gl_NamedBufferSubData)(GLuint buffer, GLintptr offset,
                                     GLsizeiptr size, const void *data);
extern void (*gl_ProgramUniform1i)(GLuint program, GLint location, GLint value);
extern void (*gl_ProgramUniform3f)(GLuint program, GLint location, GLfloat v0,
                                   GLfloat v1, GLfloat v2);
extern void (*gl_ProgramUniform3i)(GLuint program, GLint location, GLint v0,
                                   GLint v1, GLint v2);
extern void (*gl_ProgramUniformMatrix4fv)(GLuint program, GLint location,
                                          GLsizei count, GLboolean transpose,
                                          const GLfloat *value);
extern void (*gl_ShaderSource)(GLuint shader, GLsizei count, GLchar **string,
                               GLint *length);
extern void (*gl_UseProgram)(GLuint program);
extern void (*gl_VertexArrayAttribBinding)(GLuint vaobj, GLuint attribindex,
                                           GLuint bindingindex);
extern void (*gl_VertexArrayAttribFormat)(GLuint vaobj, GLuint attribindex,
                                          GLint size, GLenum type,
                                          GLboolean normalized,
                                          GLuint relativeoffset);
extern void (*gl_VertexArrayVertexBuffer)(GLuint vaobj, GLuint bindingindex,
                                          GLuint buffer, GLintptr offset,
                                          GLsizei stride);
extern void (*gl_Viewport)(GLint x, GLint y, GLsizei width, GLsizei height);

void gl_loadproc(void);
void gl_loadproc_minimal(void);

#endif // _GL_H_
