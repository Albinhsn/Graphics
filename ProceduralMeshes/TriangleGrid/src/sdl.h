#ifndef STA_LIB_SDL_H
#define STA_LIB_SDL_H
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

// Init SDL/OpenGL
// Create a window and a context
SDL_Window *sta_initSDLWindow(SDL_GLContext *context, int screenWidth,
                              int screenHeight);
void sta_createGLProgram();
// Import every function i need
// TYPE DEFINITIONS
typedef GLuint(APIENTRY *PFNGLCREATESHADERPROC)(GLenum type);
typedef void(APIENTRY *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void(APIENTRY *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname,
                                             GLint *params);
typedef void(APIENTRY *PFNGLGETSHADERINFOLOGPROC)(GLuint shader,
                                                  GLsizei bufSize,
                                                  GLsizei *length,
                                                  char *infoLog);
typedef GLuint(APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void(APIENTRY *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void(APIENTRY *PFNGLBINDATTRIBLOCATIONPROC)(GLuint program,
                                                    GLuint index,
                                                    const char *name);
typedef void(APIENTRY *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void(APIENTRY *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname,
                                              GLint *params);
typedef void(APIENTRY *PFNGLGETPROGRAMINFOLOGPROC)(GLuint program,
                                                   GLsizei bufSize,
                                                   GLsizei *length,
                                                   char *infoLog);
typedef void(APIENTRY *PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void(APIENTRY *PFNGLDELETESHADERPROC)(GLuint shader);
typedef void(APIENTRY *PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void(APIENTRY *PFNGLUSEPROGRAMPROC)(GLuint program);
typedef GLint(APIENTRY *PFNGLGETUNIFORMLOCATIONPROC)(GLuint program,
                                                     const char *name);
typedef void(APIENTRY *PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count,
                                                  GLboolean transpose,
                                                  const GLfloat *value);
typedef void(APIENTRY *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void(APIENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void(APIENTRY *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void(APIENTRY *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void(APIENTRY *PFNGLBUFFERDATAPROC)(GLenum target, ptrdiff_t size,
                                            const GLvoid *data, GLenum usage);
typedef void(APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void(APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size,
                                                     GLenum type,
                                                     GLboolean normalized,
                                                     GLsizei stride,
                                                     const GLvoid *pointer);
typedef void(APIENTRY *PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void(APIENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei n,
                                               const GLuint *buffers);
typedef void(APIENTRY *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n,
                                                    const GLuint *arrays);
typedef void(APIENTRY *PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void(APIENTRY *PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void(APIENTRY *PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void(APIENTRY *PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count,
                                            const GLfloat *value);
typedef void(APIENTRY *PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count,
                                            const GLfloat *value);
typedef void(APIENTRY *PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count,
                                            const GLfloat *value);
typedef void *(APIENTRY *PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
typedef GLboolean(APIENTRY *PFNGLUNMAPBUFFERPROC)(GLenum target);
typedef void(APIENTRY *PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void(APIENTRY *PFNGLGENFRAMEBUFFERSPROC)(GLsizei n,
                                                 GLuint *framebuffers);
typedef void(APIENTRY *PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n,
                                                    const GLuint *framebuffers);
typedef void(APIENTRY *PFNGLBINDFRAMEBUFFERPROC)(GLenum target,
                                                 GLuint framebuffer);
typedef void(APIENTRY *PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target,
                                                      GLenum attachment,
                                                      GLenum textarget,
                                                      GLuint texture,
                                                      GLint level);
typedef void(APIENTRY *PFNGLGENRENDERBUFFERSPROC)(GLsizei n,
                                                  GLuint *renderbuffers);
typedef void(APIENTRY *PFNGLBINDRENDERBUFFERPROC)(GLenum target,
                                                  GLuint renderbuffer);
typedef void(APIENTRY *PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target,
                                                     GLenum internalformat,
                                                     GLsizei width,
                                                     GLsizei height);
typedef void(APIENTRY *PFNGLFRAMEBUFFERRENDERBUFFERPROC)(
    GLenum target, GLenum attachment, GLenum renderbuffertarget,
    GLuint renderbuffer);
typedef void(APIENTRY *PFNGLDRAWBUFFERSARBPROC)(GLsizei n, const GLenum *bufs);
typedef void(APIENTRY *PFNGLDELETERENDERBUFFERSPROC)(
    GLsizei n, const GLuint *renderbuffers);
typedef void(APIENTRYP PFNGLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB,
                                                   GLenum dfactorRGB,
                                                   GLenum sfactorAlpha,
                                                   GLenum dfactorAlpha);

GLuint glCreateShader(GLuint shaderType);
void glShaderSource(GLuint shader, GLsizei count,
                        const GLchar *const *string, const GLint *length);
void glCompileShader(GLuint shader);
void glGetShaderiv(GLuint shader, GLuint pname, GLint *status);
char *glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length,
                         char *infoLog);
GLuint glCreateProgram();
void glAttachShader(GLuint shaderProgram, GLuint vShader);
void glBindAttribLocation(GLuint program, GLuint index, const char *name);
void glLinkProgram(GLuint shaderProgram);
void glGetProgramiv(GLuint shaderProgram, GLuint pname, int *status);
char *glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length,
                          char *infoLog);
void glDetachShader(GLuint program, GLuint shader);
void glDeleteShader(GLuint shader);
void glDeleteProgram(GLuint program);
void glUseProgram(GLuint program);
int glGetUniformLocation(GLuint program, const char *variableName);
void glUniformMatrix4fv(GLint location, GLint count, GLboolean transpose,
                        const GLfloat *matrix);
void glGenVertexArrays(GLint n, GLuint *buffer);
void glBindVertexArray(GLuint buffer);
void glGenBuffers(GLint n, GLuint *buffer);
void glBindBuffer(GLenum target, GLuint buffer);
void glBufferData(GLenum target, size_t size, void *data, GLenum usage);
void glEnableVertexAttribArray(GLuint index);
void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
                           GLboolean normalized, GLsizei stride,
                           const GLvoid *pointer);
void glDisableVertexAttribArray(GLuint index);
void glDeleteBuffers(GLint n, GLuint *buffers);
void glDeleteVertexArrays(GLint n, GLuint *arrays);
void glUniform1i(GLint location, GLint v0);
void glGenerateMipmap(GLenum target);
void glUniform2fv(GLint location, GLint count, float *value);
void glUniform3fv(GLint location, GLint count, float *value);
void glUniform4fv(GLint location, GLint count, float *value);
void *glMapBuffer(GLenum target, GLenum access);
void glUnmapBuffer(GLenum target);
void glUniform1f(GLint location, GLfloat v0);
void glGenFramebuffers(GLint n, GLuint *framebuffers);
void glDeleteFrameBuffers(GLint n, GLuint *framebuffers);
void glBindFramebuffer(GLenum target, GLuint framebuffer);
void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void glGenRenderbuffers(GLint n, GLuint *renderbuffers);
void glBindRenderBuffer(GLenum target, GLuint framebuffer);
void glRenderbufferStorage(GLenum target, GLenum internalformat, GLint width, GLint height);
void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void glDrawBuffers(GLint n, const GLenum *bufs);
void glDeleteRenderbuffers(GLint n, GLuint *renderbuffers);
void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

#endif
