#include "sdl.h"
#include <GL/glext.h>
#include <SDL2/SDL_video.h>

SDL_Window *initSDLWindow(SDL_GLContext *context, int screenWidth,
                          int screenHeight) {
  SDL_Window *window;

  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("client", 0, 0, screenWidth, screenHeight,
                            SDL_WINDOW_OPENGL);
  *context = SDL_GL_CreateContext(window);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  return window;
}
GLuint glCreateShader(GLuint shaderType) {
  PFNGLCREATESHADERPROC glCreateShader =
      (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
  if (!glCreateShader) {
    printf("ERROR: Unable to cast glCreateShader\n");
    exit(2);
  }
  return glCreateShader(shaderType);
}
void glShaderSource(GLuint shader, GLsizei count, const GLchar *const *string,
                    const GLint *length) {
  PFNGLSHADERSOURCEPROC glShaderSource =
      (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
  if (!glShaderSource) {
    printf("ERROR: Unable to cast glShaderSource\n");
    exit(2);
  }
  glShaderSource(shader, count, string, length);
}
void glCompileShader(GLuint shader) {

  PFNGLCOMPILESHADERPROC glCompileShader =
      (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
  if (!glCompileShader) {
    printf("ERROR: Unable to cast glCompileShader\n");
    exit(2);
  }
  glCompileShader(shader);
}
void glGetShaderiv(GLuint shader, GLuint pname, GLint *status) {
  PFNGLGETSHADERIVPROC glGetShaderiv =
      (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
  if (!glGetShaderiv) {
    printf("ERROR: Unable to cast glGetShaderiv\n");
    exit(2);
  }
  glGetShaderiv(shader, pname, status);
}
char *glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length,
                         char *infoLog) {
  PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog =
      (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
  if (!glGetShaderInfoLog) {
    printf("ERROR: Unable to cast glGetShaderInfoLog\n");
    exit(2);
  }
  glGetShaderInfoLog(shader, bufSize, NULL, infoLog);

  return infoLog;
}
GLuint glCreateProgram() {
  PFNGLCREATEPROGRAMPROC glCreateProgram =
      (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
  if (!glCreateProgram) {
    printf("ERROR: Unable to cast glCreateProgram\n");
    exit(2);
  }
  return glCreateProgram();
}
void glBindAttribLocation(GLuint program, GLuint index, const char *name) {
  PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation =
      (PFNGLBINDATTRIBLOCATIONPROC)SDL_GL_GetProcAddress(
          "glBindAttribLocation");
  if (!glBindAttribLocation) {
    printf("ERROR: Unable to cast bindAttribLocation\n");
    exit(2);
  }

  glBindAttribLocation(program, index, name);
}
void glLinkProgram(GLuint program) {
  PFNGLLINKPROGRAMPROC glLinkProgram =
      (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
  if (!glLinkProgram) {
    printf("ERROR: Unable to cast glLinkProgram\n");
    exit(2);
  }
  glLinkProgram(program);
}
void glGetProgramiv(GLuint program, GLenum pname, GLint *params) {

  PFNGLGETPROGRAMIVPROC glGetProgramiv =
      (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
  if (!glGetProgramiv) {
    printf("ERROR: Unable to cast glGetProgramiv\n");
    exit(2);
  }
  glGetProgramiv(program, pname, params);
}
char *glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length,
                          char *infoLog) {
  PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog =
      (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
  if (!glGetProgramInfoLog) {
    printf("ERROR: Unable to cast GetProgramInfoLog\n");
    exit(2);
  }
  glGetProgramInfoLog(program, bufSize, length, infoLog);

  return infoLog;
}
void glDetachShader(GLuint program, GLuint shader) {

  PFNGLDETACHSHADERPROC glDetachShader =
      (PFNGLDETACHSHADERPROC)SDL_GL_GetProcAddress("glDetachShader");
  if (!glDetachShader) {
    printf("ERROR: Unable to cast glDetachShader\n");
    exit(2);
  }
  glDetachShader(program, shader);
}
void glDeleteShader(GLuint shader) {
  PFNGLDELETESHADERPROC glDeleteShader =
      (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
  if (!glDeleteShader) {
    printf("ERROR: Unable to cast glDeleteShader\n");
    exit(2);
  }
  glDeleteShader(shader);
}
void glDeleteProgram(GLuint program) {
  PFNGLDELETEPROGRAMPROC glDeleteProgram =
      (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress("glDeleteProgram");
  if (!glDeleteProgram) {
    printf("ERROR: Unable to cast glDeleteShader\n");
    exit(2);
  }
  glDeleteProgram(program);
}
void glUseProgram(GLuint program) {

  PFNGLUSEPROGRAMPROC glUseProgram =
      (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
  if (!glUseProgram) {
    printf("ERROR: Unable to cast glDeleteShader\n");
    exit(2);
  }
  glUseProgram(program);
}
void glAttachShader(GLuint shaderProgram, GLuint vShader){
  PFNGLATTACHSHADERPROC glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
  if(!glAttachShader){
    printf("ERROR: Unable to cast glAttachShader");
    exit(2);
  }
  glAttachShader(shaderProgram, vShader);

}
int glGetUniformLocation(GLuint program, const char *variableName) {
  PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation =
      (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress(
          "glGetUniformLocation");
  if (!glGetUniformLocation) {
    printf("ERROR: Unable to cast glGetUniformLocation\n");
    exit(2);
  }
  return glGetUniformLocation(program, variableName);
}
void glUniformMatrix4fv(GLint location, GLint count, GLboolean transpose,
                        const GLfloat *matrix) {
  PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv =
      (PFNGLUNIFORMMATRIX4FVPROC)SDL_GL_GetProcAddress("glUniformMatrix4fv");
  if (!glUniformMatrix4fv) {
    printf("ERROR: Unable to cast glUniformMatrix4fv\n");
    exit(2);
  }
  glUniformMatrix4fv(location, count, transpose, matrix);
}
void glGenVertexArrays(GLsizei n, GLuint *buffer) {
  PFNGLGENVERTEXARRAYSPROC glGenVertexArrays =
      (PFNGLGENVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glGenVertexArrays");
  if (!glGenVertexArrays) {
    printf("ERROR: Unable to cast glGenVertexArrays\n");
    exit(2);
  }
  glGenVertexArrays(n, buffer);
}
void glBindVertexArray(GLuint buffer) {

  PFNGLBINDVERTEXARRAYPROC glBindVertexArray =
      (PFNGLBINDVERTEXARRAYPROC)SDL_GL_GetProcAddress("glBindVertexArray");
  if (!glBindVertexArray) {
    printf("ERROR: Unable to cast glBindVertexArray\n");
    exit(2);
  }
  glBindVertexArray(buffer);
}
void glGenBuffers(GLint n, GLuint *buffer) {

  PFNGLGENBUFFERSPROC glGenBuffers =
      (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers");
  if (!glGenBuffers) {
    printf("ERROR: Unable to cast glGenBuffers\n");
    exit(2);
  }
  glGenBuffers(n, buffer);
}
void glBindBuffer(GLenum target, GLuint buffer) {

  PFNGLBINDBUFFERPROC glBindBuffer =
      (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
  if (!glBindBuffer) {
    printf("ERROR: Unable to cast glBindBuffer\n");
    exit(2);
  }
  glBindBuffer(target, buffer);
}
void glBufferData(GLenum target, size_t size, void *data, GLenum usage) {

  PFNGLBUFFERDATAPROC glBufferData =
      (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData");
  if (!glBufferData) {
    printf("ERROR: Unable to cast glBufferData\n");
    exit(2);
  }
  glBufferData(target, size, data, usage);
}
void glEnableVertexAttribArray(GLuint index) {

  PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray =
      (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress(
          "glEnableVertexAttribArray");
  if (!glEnableVertexAttribArray) {
    printf("ERROR: Unable to cast glEnableVertexAttribArray\n");
    exit(2);
  }
  glEnableVertexAttribArray(index);
}
void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
                           GLboolean normalized, GLsizei stride,
                           const GLvoid *pointer) {

  PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer =
      (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress(
          "glVertexAttribPointer");
  if (!glVertexAttribPointer) {
    printf("ERROR: Unable to cast glVertexAttribArray\n");
    exit(2);
  }
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}
void glDisableVertexAttribArray(GLuint index) {

  PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray =
      (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress(
          "glDisableVertexAttribArray");
  if (!glDisableVertexAttribArray) {
    printf("ERROR: Unable to cast glDisableVertexAttribArray\n");
    exit(2);
  }
  glDisableVertexAttribArray(index);
}
void glDeleteBuffers(GLint n, GLuint *buffers) {

  PFNGLDELETEBUFFERSPROC glDeleteBuffers =
      (PFNGLDELETEBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteBuffers");
  if (!glDeleteBuffers) {
    printf("ERROR: Unable to cast glDeleteBuffers\n");
    exit(2);
  }
  glDeleteBuffers(n, buffers);
}
void glDeleteVertexArrays(GLint n, GLuint *arrays) {

  PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays =
      (PFNGLDELETEVERTEXARRAYSPROC)SDL_GL_GetProcAddress(
          "glDeleteVertexArrays");
  if (!glDeleteVertexArrays) {
    printf("ERROR: Unable to cast glDeleteVertexArrays\n");
    exit(2);
  }
  glDeleteVertexArrays(n, arrays);
}
void glUniform1i(GLint location, GLint v0) {

  PFNGLUNIFORM1IPROC glUniform1i =
      (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");
  if (!glUniform1i) {
    printf("ERROR: Unable to cast glUniform1i\n");
    exit(2);
  }
  glUniform1i(location, v0);
}
void glGenerateMipmap(GLenum target) {

  PFNGLGENERATEMIPMAPPROC glGenerateMipmap =
      (PFNGLGENERATEMIPMAPPROC)SDL_GL_GetProcAddress("glGenerateMipmap");
  if (!glGenerateMipmap) {
    printf("ERROR: Unable to cast glGenerateMipmap\n");
    exit(2);
  }
  glGenerateMipmap(target);
}
void glUniform2fv(GLint location, GLint count, GLfloat *value) {

  PFNGLUNIFORM2FVPROC glUniform2fv =
      (PFNGLUNIFORM2FVPROC)SDL_GL_GetProcAddress("glUniform2fv");
  if (!glUniform2fv) {
    printf("ERROR: Unable to cast glUniform2fv\n");
    exit(2);
  }
  glUniform2fv(location, count, value);
}
void glUniform3fv(GLint location, GLint count, GLfloat *value) {

  PFNGLUNIFORM3FVPROC glUniform3fv =
      (PFNGLUNIFORM3FVPROC)SDL_GL_GetProcAddress("glUniform3fv");
  if (!glUniform3fv) {
    printf("ERROR: Unable to cast glUniform3fv\n");
    exit(2);
  }
  glUniform3fv(location, count, value);
}
void glUniform4fv(GLint location, GLint count, GLfloat *value) {

  PFNGLUNIFORM4FVPROC glUniform4fv =
      (PFNGLUNIFORM4FVPROC)SDL_GL_GetProcAddress("glUniform4fv");
  if (!glUniform4fv) {
    printf("ERROR: Unable to cast glUniform4fv\n");
    exit(2);
  }
  glUniform4fv(location, count, value);
}
void *glMapBuffer(GLenum target, GLenum access) {

  PFNGLMAPBUFFERPROC glMapBuffer =
      (PFNGLMAPBUFFERPROC)SDL_GL_GetProcAddress("glMapBuffer");
  if (!glMapBuffer) {
    printf("ERROR: Unable to cast glMapBuffer\n");
    exit(2);
  }
  return glMapBuffer(target, access);
}
void glUnmapBuffer(GLenum target) {

  PFNGLUNMAPBUFFERPROC glUnmapBuffer =
      (PFNGLUNMAPBUFFERPROC)SDL_GL_GetProcAddress("glUnmapBuffer");
  if (!glUnmapBuffer) {
    printf("ERROR: Unable to cast glUnmapBuffer\n");
    exit(2);
  }
  glUnmapBuffer(target);
}
void glUniform1f(GLint location, GLfloat v0) {

  PFNGLUNIFORM1FPROC glUniform1f =
      (PFNGLUNIFORM1FPROC)SDL_GL_GetProcAddress("glUniform1f");
  if (!glUniform1f) {
    printf("ERROR: Unable to cast glUniform1f\n");
    exit(2);
  }
  glUniform1f(location, v0);
}
void glGenFramebuffers(GLint n, GLuint *framebuffers) {

  PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers =
      (PFNGLGENFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glGenFramebuffers");
  if (!glGenFramebuffers) {
    printf("ERROR: Unable to cast glGenFramebuffers\n");
    exit(2);
  }
  glGenFramebuffers(n, framebuffers);
}
void glDeleteFrameBuffers(GLint n, GLuint *framebuffers) {

  PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers =
      (PFNGLDELETEFRAMEBUFFERSPROC)SDL_GL_GetProcAddress(
          "glDeleteFramebuffers");
  if (!glDeleteFramebuffers) {
    printf("ERROR: Unable to cast glDeleteFramebuffers\n");
    exit(2);
  }
  glDeleteFramebuffers(n, framebuffers);
}
void glBindFramebuffer(GLenum target, GLuint framebuffer) {

  PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer =
      (PFNGLBINDFRAMEBUFFERPROC)SDL_GL_GetProcAddress("glBindFramebuffer");
  if (!glBindFramebuffer) {
    printf("ERROR: Unable to cast glBindFramebuffer\n");
    exit(2);
  }
  glBindFramebuffer(target, framebuffer);
}
void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget,
                            GLuint texture, int level) {

  PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D =
      (PFNGLFRAMEBUFFERTEXTURE2DPROC)SDL_GL_GetProcAddress(
          "glFramebufferTexture2D");
  if (!glFramebufferTexture2D) {
    printf("ERROR: Unable to cast glFramebufferTexture2D\n");
    exit(2);
  }
  glFramebufferTexture2D(target, attachment, textarget, texture, level);
}
void glGenRenderbuffers(GLint n, GLuint *renderbuffers) {

  PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers =
      (PFNGLGENRENDERBUFFERSPROC)SDL_GL_GetProcAddress("glGenRenderbuffers");
  if (!glGenRenderbuffers) {
    printf("ERROR: Unable to cast glGenRenderbuffer\n");
    exit(2);
  }
  glGenRenderbuffers(n, renderbuffers);
}
void glBindRenderBuffer(GLenum target, GLuint framebuffer) {

  PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer =
      (PFNGLBINDRENDERBUFFERPROC)SDL_GL_GetProcAddress("glBindRenderbuffer");
  if (!glBindRenderbuffer) {
    printf("ERROR: Unable to cast glBindRenderBuffer\n");
    exit(2);
  }
  glBindRenderbuffer(target, framebuffer);
}
void glRenderbufferStorage(GLenum target, GLenum internalformat, GLint width,
                           GLint height) {

  PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage =
      (PFNGLRENDERBUFFERSTORAGEPROC)SDL_GL_GetProcAddress(
          "glRenderbufferStorage");
  if (!glRenderbufferStorage) {
    printf("ERROR: Unable to cast glBindRenderbufferStorage\n");
    exit(2);
  }
  glRenderbufferStorage(target, internalformat, width, height);
}
void glFramebufferRenderbuffer(GLenum target, GLenum attachment,
                               GLenum renderbuffertarget, GLuint renderbuffer) {

  PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer =
      (PFNGLFRAMEBUFFERRENDERBUFFERPROC)SDL_GL_GetProcAddress(
          "glFramebufferRenderbuffer");
  if (!glFramebufferRenderbuffer) {
    printf("ERROR: Unable to cast glFramebufferRenderbuffer\n");
    exit(2);
  }
  glFramebufferRenderbuffer(target, attachment, renderbuffertarget,
                            renderbuffer);
}
void glDrawBuffers(GLint n, const GLenum *bufs) {

  PFNGLDRAWBUFFERSARBPROC glDrawBuffers =
      (PFNGLDRAWBUFFERSARBPROC)SDL_GL_GetProcAddress("glDrawBuffers");
  if (!glDrawBuffers) {
    printf("ERROR: Unable to cast glDrawBuffers\n");
    exit(2);
  }
  glDrawBuffers(n, bufs);
}
void glDeleteRenderbuffers(GLint n, GLuint *renderbuffers) {

  PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers =
      (PFNGLDELETERENDERBUFFERSPROC)SDL_GL_GetProcAddress(
          "glDeleteRenderbuffers");
  if (!glDeleteRenderbuffers) {
    printf("ERROR: Unable to cast glDeleteRenderbuffers\n");
    exit(2);
  }
  glDeleteRenderbuffers(n, renderbuffers);
}
void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB,
                         GLenum sfactorAlpha, GLenum dfactorAlpha) {

  PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate =
      (PFNGLBLENDFUNCSEPARATEPROC)SDL_GL_GetProcAddress("glBlendFuncSeparate");
  if (!glBlendFuncSeparate) {
    printf("ERROR: Unable to cast glBlendFuncSeparate\n");
    exit(2);
  }
  glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}
