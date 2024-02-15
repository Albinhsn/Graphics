#include "sdl.h"
#include "common.h"
#include "vector.h"

PFNGLCREATESHADERPROC             glCreateShader             = NULL;
PFNGLCOMPILESHADERPROC            glCompileShader            = NULL;
PFNGLGETSHADERIVPROC              glGetShaderiv              = NULL;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog         = NULL;
PFNGLCREATEPROGRAMPROC            glCreateProgram            = NULL;
PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation       = NULL;
PFNGLLINKPROGRAMPROC              glLinkProgram              = NULL;
PFNGLSHADERSOURCEPROC             glShaderSource             = NULL;
PFNGLGETPROGRAMIVPROC             glGetProgramiv             = NULL;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog        = NULL;
PFNGLDETACHSHADERPROC             glDetachShader             = NULL;
PFNGLDELETESHADERPROC             glDeleteShader             = NULL;
PFNGLDELETEPROGRAMPROC            glDeleteProgram            = NULL;
PFNGLUSEPROGRAMPROC               glUseProgram               = NULL;
PFNGLATTACHSHADERPROC             glAttachShader             = NULL;
PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation       = NULL;
PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv         = NULL;
PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays          = NULL;
PFNGLGENBUFFERSPROC               glGenBuffers               = NULL;
PFNGLBINDBUFFERPROC               glBindBuffer               = NULL;
PFNGLBUFFERDATAPROC               glBufferData               = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray  = NULL;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer      = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLDELETEBUFFERSPROC            glDeleteBuffers            = NULL;
PFNGLDELETEVERTEXARRAYSPROC       glDeleteVertexArrays       = NULL;
PFNGLUNIFORM1IPROC                glUniform1i                = NULL;
PFNGLGENERATEMIPMAPPROC           glGenerateMipmap           = NULL;
PFNGLUNIFORM2FVPROC               glUniform2fv               = NULL;
PFNGLUNIFORM3FVPROC               glUniform3fv               = NULL;
PFNGLUNIFORM4FVPROC               glUniform4fv               = NULL;
PFNGLMAPNAMEDBUFFERPROC           glMapNamedBuffer           = NULL;
PFNGLMAPBUFFERPROC                glMapBuffer                = NULL;
PFNGLUNMAPBUFFERPROC              glUnmapBuffer              = NULL;
PFNGLUNIFORM1FPROC                glUniform1f                = NULL;
PFNGLGENFRAMEBUFFERSPROC          glGenFramebuffers          = NULL;
PFNGLDELETEFRAMEBUFFERSPROC       glDeleteFramebuffers       = NULL;
PFNGLBINDFRAMEBUFFERPROC          glBindFramebuffer          = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC     glFramebufferTexture2D     = NULL;
PFNGLGENRENDERBUFFERSPROC         glGenRenderbuffers         = NULL;
PFNGLBINDRENDERBUFFERPROC         glBindRenderbuffer         = NULL;
PFNGLRENDERBUFFERSTORAGEPROC      glRenderbufferStorage      = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC  glFramebufferRenderbuffer  = NULL;
PFNGLDRAWBUFFERSARBPROC           glDrawBuffers              = NULL;
PFNGLDELETERENDERBUFFERSPROC      glDeleteRenderbuffers      = NULL;
PFNGLBLENDFUNCSEPARATEPROC        glBlendFuncSeparate        = NULL;
PFNGLBINDVERTEXARRAYPROC          glBindVertexArray          = NULL;

void                              loadExtensions()
{
  glCreateShader             = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
  glCompileShader            = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
  glGetShaderiv              = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
  glGetShaderInfoLog         = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
  glCreateProgram            = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
  glBindAttribLocation       = (PFNGLBINDATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glBindAttribLocation");
  glLinkProgram              = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
  glShaderSource             = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
  glLinkProgram              = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
  glGetProgramiv             = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
  glGetProgramInfoLog        = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
  glDetachShader             = (PFNGLDETACHSHADERPROC)SDL_GL_GetProcAddress("glDetachShader");
  glDeleteShader             = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
  glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress("glDeleteProgram");
  glUseProgram               = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
  glAttachShader             = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
  glGetUniformLocation       = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");
  glUniformMatrix4fv         = (PFNGLUNIFORMMATRIX4FVPROC)SDL_GL_GetProcAddress("glUniformMatrix4fv");
  glGenVertexArrays          = (PFNGLGENVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glGenVertexArrays");
  glBindVertexArray          = (PFNGLBINDVERTEXARRAYPROC)SDL_GL_GetProcAddress("glBindVertexArray");
  glGenBuffers               = (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers");
  glBindBuffer               = (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
  glBufferData               = (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData");
  glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
  glVertexAttribPointer      = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
  glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glDisableVertexAttribArray");
  glDeleteBuffers            = (PFNGLDELETEBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteBuffers");
  glDeleteVertexArrays       = (PFNGLDELETEVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glDeleteVertexArrays");
  glUniform1i                = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");
  glGenerateMipmap           = (PFNGLGENERATEMIPMAPPROC)SDL_GL_GetProcAddress("glGenerateMipmap");
  glUniform2fv               = (PFNGLUNIFORM2FVPROC)SDL_GL_GetProcAddress("glUniform2fv");
  glUniform3fv               = (PFNGLUNIFORM3FVPROC)SDL_GL_GetProcAddress("glUniform3fv");
  glUniform4fv               = (PFNGLUNIFORM4FVPROC)SDL_GL_GetProcAddress("glUniform4fv");
  glMapNamedBuffer           = (PFNGLMAPNAMEDBUFFERPROC)SDL_GL_GetProcAddress("glMapNamedBuffer");
  glMapBuffer                = (PFNGLMAPBUFFERPROC)SDL_GL_GetProcAddress("glMapBuffer");
  glUnmapBuffer              = (PFNGLUNMAPBUFFERPROC)SDL_GL_GetProcAddress("glUnmapBuffer");
  glUniform1f                = (PFNGLUNIFORM1FPROC)SDL_GL_GetProcAddress("glUniform1f");
  glGenFramebuffers          = (PFNGLGENFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glGenFramebuffers");
  glDeleteFramebuffers       = (PFNGLDELETEFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteFramebuffers");
  glBindFramebuffer          = (PFNGLBINDFRAMEBUFFERPROC)SDL_GL_GetProcAddress("glBindFramebuffer");
  glFramebufferTexture2D     = (PFNGLFRAMEBUFFERTEXTURE2DPROC)SDL_GL_GetProcAddress("glFramebufferTexture2D");
  glGenRenderbuffers         = (PFNGLGENRENDERBUFFERSPROC)SDL_GL_GetProcAddress("glGenRenderbuffers");
  glBindRenderbuffer         = (PFNGLBINDRENDERBUFFERPROC)SDL_GL_GetProcAddress("glBindRenderbuffer");
  glRenderbufferStorage      = (PFNGLRENDERBUFFERSTORAGEPROC)SDL_GL_GetProcAddress("glRenderbufferStorage");
  glFramebufferRenderbuffer  = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)SDL_GL_GetProcAddress("glFramebufferRenderbuffer");
  glDrawBuffers              = (PFNGLDRAWBUFFERSARBPROC)SDL_GL_GetProcAddress("glDrawBuffers");
  glDeleteRenderbuffers      = (PFNGLDELETERENDERBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteRenderbuffers");
  glBlendFuncSeparate        = (PFNGLBLENDFUNCSEPARATEPROC)SDL_GL_GetProcAddress("glBlendFuncSeparate");
}

GLuint sta_glCreateShader(GLenum type)
{
  return glCreateShader(type);
}
void sta_glCompileShader(GLuint shader)
{
  glCompileShader(shader);
}
void sta_glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
  glGetShaderiv(shader, pname, params);
}
void sta_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, char* infoLog)
{
  glGetShaderInfoLog(shader, bufSize, length, infoLog);
}
GLuint sta_glCreateProgram()
{
  return glCreateProgram();
}
void sta_glAttachShader(GLuint program, GLuint shader)
{
  glAttachShader(program, shader);
}
void sta_glBindAttribLocation(GLuint program, GLuint index, const char* name)
{
  glBindAttribLocation(program, index, name);
}
void sta_glLinkProgram(GLuint program)
{
  glLinkProgram(program);
}
void sta_glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
  glGetProgramiv(program, pname, params);
}
void sta_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, char* infoLog)
{
  glGetProgramInfoLog(program, bufSize, length, infoLog);
}
void sta_glDetachShader(GLuint program, GLuint shader)
{
  glDetachShader(program, shader);
}
void sta_glDeleteProgram(GLuint program)
{
  glDeleteProgram(program);
}
void sta_glUseProgram(GLuint program)
{
  glUseProgram(program);
}
GLint sta_glGetUniformLocation(GLuint program, const char* name)
{
  return glGetUniformLocation(program, name);
}
void sta_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
  glUniformMatrix4fv(location, count, transpose, value);
}
void sta_glGenVertexArrays(GLsizei n, GLuint* arrays)
{
  glGenVertexArrays(n, arrays);
}
void sta_glBindVertexArray(GLuint array)
{
  glBindVertexArray(array);
}
void sta_glGenBuffers(GLsizei n, GLuint* buffers)
{
  glGenBuffers(n, buffers);
}
void sta_glBindBuffer(GLenum target, GLuint buffer)
{
  glBindBuffer(target, buffer);
}
void sta_glBufferData(GLenum target, ptrdiff_t size, const GLvoid* data, GLenum usage)
{
  glBufferData(target, size, data, usage);
}
void sta_glEnableVertexAttribArray(GLuint index)
{
  glEnableVertexAttribArray(index);
}
void sta_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}
void sta_glDisableVertexAttribArray(GLuint index)
{
  glDisableVertexAttribArray(index);
}
void sta_glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
  glDeleteBuffers(n, buffers);
}
void sta_glDeleteVertexArrays(GLsizei n, const GLuint* arrays)
{
  glDeleteVertexArrays(n, arrays);
}
void sta_glUniform1i(GLint location, GLint v0)
{
  glUniform1i(location, v0);
}
void sta_glGenerateMipmap(GLenum target)
{
  glGenerateMipmap(target);
}
void sta_glUniform2fv(GLint location, GLsizei count, const GLfloat* value)
{
  glUniform2fv(location, count, value);
}
void sta_glUniform3fv(GLint location, GLsizei count, const GLfloat* value)
{
  glUniform3fv(location, count, value);
}
void sta_glUniform4fv(GLint location, GLsizei count, const GLfloat* value)
{
  glUniform4fv(location, count, value);
}
void* sta_glMapBuffer(GLenum target, GLenum access)
{
  return glMapBuffer(target, access);
}
GLboolean sta_glUnmapBuffer(GLenum target)
{
  return glUnmapBuffer(target);
}
void sta_glUniform1f(GLint location, GLfloat v0)
{
  glUniform1f(location, v0);
}
void sta_glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
  glGenFramebuffers(n, framebuffers);
}
void sta_glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
  glDeleteFramebuffers(n, framebuffers);
}
void sta_glBindFramebuffer(GLenum target, GLuint framebuffer)
{
  glBindFramebuffer(target, framebuffer);
}
void sta_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  glFramebufferTexture2D(target, attachment, textarget, texture, level);
}
void sta_glGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
  glGenRenderbuffers(n, renderbuffers);
}
void sta_glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
  glBindRenderbuffer(target, renderbuffer);
}
void sta_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
  glRenderbufferStorage(target, internalformat, width, height);
}
void sta_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}
void sta_glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
  glDeleteRenderbuffers(n, renderbuffers);
}
void sta_glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
  glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}
void sta_glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
{
  glShaderSource(shader, count, string, length);
}
void sta_glMapNamedBuffer(GLuint buffer, GLenum access)
{
  glMapNamedBuffer(buffer, access);
}
void sta_glDrawBuffers(GLint n, const GLenum* bufs)
{
  glDrawBuffers(n, bufs);
}

static inline void setTexture(GLuint textureId)
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);
}

static inline void setTextShaderParams(GLuint program, struct Vec4f32* color)
{
  glUseProgram(program);
  int location = glGetUniformLocation(program, "shaderTexture");
  if (location == -1)
  {
    printf("Failed to finder shaderTexture when rendering text\n");
    exit(1);
  }
  glUniform1i(location, 0);

  // Set the font pixel color in the pixel shader.
  location = glGetUniformLocation(program, "pixelColor");
  if (location == -1)
  {
    printf("Failed to finder pixelColor when rendering text\n");
    exit(1);
  }
  f32 colorf32[4] = {color->pos[0], color->pos[1], color->pos[2], color->pos[3]};
  glUniform4fv(location, 1, &colorf32[0]);
}

static void buildUpdatedTextVertexArray(struct Font* font, GLfloat* vertices, const char* text, f32 x, f32 y, f32 fontSize)
{
  GLfloat drawX, drawY;
  drawX          = WORLDSPACEX_TO_VIEWSPACE(x);
  drawY          = WORLDSPACEY_TO_VIEWSPACE(y + fontSize / 4.0f);

  i32 numLetters = (i32)strlen(text);
  f32 height     = WORLDSPACEY_TO_VIEWSPACE(fontSize);
  f32 totalSize  = 0;

  for (i32 i = 0; i < numLetters; i++)
  {
    i32 letter = ((i32)text[i]) - 32;
    totalSize += WORLDSPACEX_TO_VIEWSPACE(font->type[letter].size);
  }

  drawX -= totalSize / 2.0f;

  for (int letterIdx = 0, vertexIdx = 0; letterIdx < numLetters; letterIdx++)
  {
    int letter = ((int)text[letterIdx]) - 32;

    // If the letter is a space then just move over three pixels.
    if (letter == 0)
    {
      drawX = drawX + WORLDSPACEX_TO_VIEWSPACE(font->spaceSize);
    }
    else
    {
      f32 size = WORLDSPACEX_TO_VIEWSPACE(font->type[letter].size);
      // First triangle in quad.
      vertices[vertexIdx++] = drawX; // Top left.
      vertices[vertexIdx++] = drawY;
      vertices[vertexIdx++] = 0.0f;
      vertices[vertexIdx++] = font->type[letter].left;
      vertices[vertexIdx++] = 0.0f;

      vertices[vertexIdx++] = drawX + size; // Bot right.
      vertices[vertexIdx++] = drawY - height;
      vertices[vertexIdx++] = 0.0f;
      vertices[vertexIdx++] = font->type[letter].right;
      vertices[vertexIdx++] = 1.0f;

      vertices[vertexIdx++] = drawX; // Bot Left.
      vertices[vertexIdx++] = drawY - height;
      vertices[vertexIdx++] = 0.0f;
      vertices[vertexIdx++] = font->type[letter].left;
      vertices[vertexIdx++] = 1.0f;

      vertices[vertexIdx++] = drawX + size; // Top right.
      vertices[vertexIdx++] = drawY;
      vertices[vertexIdx++] = 0.0f;
      vertices[vertexIdx++] = font->type[letter].right;
      vertices[vertexIdx++] = 0.0f;

      drawX += size + 0.002f;
    }
  }
}
static void updateText(struct Font* font, const char* text, f32 x, f32 y, GLuint vertexBufferId, GLuint vertexArrayId, f32 size)
{
  int     vertexCount = TEXT_MAX_LENGTH * 4 * 5;
  GLfloat vertices[vertexCount];
  for (int i = 0; i < vertexCount; i++)
  {
    vertices[i] = 0;
  }

  glBindVertexArray(vertexArrayId);
  buildUpdatedTextVertexArray(font, vertices, text, x, y, size);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, vertices, GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void renderText(struct Font* font, const char* text, f32 x, f32 y, GLuint vertexArrayId, GLuint vertexBufferId, struct Vec4f32* color, f32 size)
{

  updateText(font, text, x, y, vertexBufferId, vertexArrayId, size);

  glBindVertexArray(vertexArrayId);
  setTextShaderParams(font->program, color);
  setTexture(font->texture);

  glDrawElements(GL_TRIANGLES, TEXT_MAX_LENGTH * 4, GL_UNSIGNED_INT, 0);
}

void updateQuadVertexArray(GLuint vertexArrayId, GLuint vertexBufferId, struct Vec2f32 xPositions, struct Vec2f32 yPositions)
{
  struct Vec2f32 viewXPositions = CREATE_VEC2f32(WORLDSPACEX_TO_VIEWSPACE(xPositions.x), WORLDSPACEX_TO_VIEWSPACE(xPositions.y));
  struct Vec2f32 viewYPositions = CREATE_VEC2f32(WORLDSPACEY_TO_VIEWSPACE(xPositions.x), WORLDSPACEY_TO_VIEWSPACE(xPositions.y));

  GLfloat        bufferData[20] = {
      viewXPositions.pos[0], viewYPositions.pos[0], 0.0f, 0.0f, 1.0f, //
      viewXPositions.pos[1], viewYPositions.pos[0], 0.0f, 1.0f, 1.0f, //
      viewXPositions.pos[0], viewYPositions.pos[1], 0.0f, 0.0f, 0.0f, //
      viewXPositions.pos[1], viewYPositions.pos[1], 0.0f, 1.0f, 0.0f  //
  };
  glBindVertexArray(vertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), bufferData, GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void createQuadVertexArray(GLuint* vertexArrayId, GLuint* vertexBufferId, struct Vec2f32 xPositions, struct Vec2f32 yPositions)
{
  struct Vec2f32 viewXPositions = CREATE_VEC2f32(WORLDSPACEX_TO_VIEWSPACE(xPositions.x), WORLDSPACEX_TO_VIEWSPACE(xPositions.y));
  struct Vec2f32 viewYPositions = CREATE_VEC2f32(WORLDSPACEY_TO_VIEWSPACE(yPositions.x), WORLDSPACEY_TO_VIEWSPACE(yPositions.y));

  GLfloat        bufferData[20] = {
      viewXPositions.pos[0], viewYPositions.pos[0], 0.0f, 0.0f, 1.0f, //
      viewXPositions.pos[1], viewYPositions.pos[0], 0.0f, 1.0f, 1.0f, //
      viewXPositions.pos[0], viewYPositions.pos[1], 0.0f, 0.0f, 0.0f, //
      viewXPositions.pos[1], viewYPositions.pos[1], 0.0f, 1.0f, 0.0f  //
  };
  int    indices[6] = {0, 1, 2, 1, 3, 2};

  GLuint indexBufferId;

  glGenVertexArrays(1, vertexArrayId);
  glBindVertexArray(*vertexArrayId);

  glGenBuffers(1, vertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), bufferData, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (signed char*)NULL + (3 * sizeof(GLfloat)));

  glGenBuffers(1, &indexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
}

static void createAndCompileShader(GLuint* shaderId, int glShaderMacro, const char* fileName)
{
  char* buffer;
  int   len;
  read_file(&buffer, &len, fileName);
  *shaderId = glCreateShader(glShaderMacro);
  glShaderSource(*shaderId, 1, &buffer, NULL);

  glCompileShader(*shaderId);

#if DEBUG_SDL
  int status;
  glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &status);
  if (status != 1)
  {
    int logSize;

    glGetShaderiv(*shaderId, GL_INFO_LOG_LENGTH, &logSize);
    logSize++;
    char infoLog[logSize];
    infoLog[logSize - 1] = '\0';

    glGetShaderInfoLog(*shaderId, logSize, NULL, infoLog);
    printf("Failed to compile shader '%s'\n", fileName);
    printf("%s\n", infoLog);
  }
  else
  {
    printf("Compiled shader '%s'\n", fileName);
  }
#endif

  free(buffer);
}

static inline void createAndCompileVertexShader(GLuint* shaderId, const char* fileName)
{
  createAndCompileShader(shaderId, GL_VERTEX_SHADER, fileName);
}

static inline void createAndCompileFragmentShader(GLuint* shaderId, const char* fileName)
{
  createAndCompileShader(shaderId, GL_FRAGMENT_SHADER, fileName);
}

void createTextBuffers(GLuint* vertexArrayId, GLuint* vertexBufferId)
{
  GLuint indexBufferId;

  int    vertexCount = TEXT_MAX_LENGTH * 4;
  GLuint indices[TEXT_MAX_LENGTH * 6];
  for (int i = 0, idx = 0; idx < TEXT_MAX_LENGTH * 6; idx += 6, i += 4)
  {
    indices[idx + 0] = i + 0;
    indices[idx + 1] = i + 1;
    indices[idx + 2] = i + 2;
    indices[idx + 3] = i + 0;
    indices[idx + 4] = i + 3;
    indices[idx + 5] = i + 1;
  }

  GLfloat vertices[vertexCount];
  for (int i = 0; i < vertexCount; i++)
  {
    vertices[i] = 0;
  }

  glGenVertexArrays(1, vertexArrayId);
  glBindVertexArray(*vertexArrayId);

  glGenBuffers(1, vertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GLfloat) * 5, 0);

  // Specify the location and format of the texture coordinate portion of the vertex buffer.
  glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(GLfloat) * 5, (unsigned char*)NULL + (3 * sizeof(float)));

  // Generate an ID for the index buffer.
  glGenBuffers(1, &indexBufferId);

  // Bind the index buffer and load the index data into it.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void createTextShaderProgram(GLuint* program)
{
  GLuint vShader, fShader;
  createAndCompileVertexShader(&vShader, "shaders/font.vs");
  createAndCompileFragmentShader(&fShader, "shaders/font.ps");

  *program = glCreateProgram();
  glAttachShader(*program, vShader);
  glAttachShader(*program, fShader);

  glBindAttribLocation(*program, 0, "inputPosition");
  glBindAttribLocation(*program, 1, "inputTexCoord");

  glLinkProgram(*program);

#if DEBUG_SDL
  int status;
  glGetProgramiv(*program, GL_LINK_STATUS, &status);
  if (status != 1)
  {
    int logSize;

    glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &logSize);
    logSize++;

    char infoLog[logSize];
    infoLog[logSize - 1] = '\0';

    glGetProgramInfoLog(*program, logSize, NULL, infoLog);
    printf("Failed to link program '%d'\n", *program);
    printf("%s\n", infoLog);
    exit(1);
  }
  else
  {
    printf("Linked program %d\n", *program);
  }
#endif
}

// ToDO clean this up to be one function
void createTextureShaderProgram(GLuint* program)
{
  GLuint vShader, fShader;
  createAndCompileVertexShader(&vShader, "shaders/texture.vs");
  createAndCompileFragmentShader(&fShader, "shaders/texture.ps");

  *program = glCreateProgram();
  glAttachShader(*program, vShader);
  glAttachShader(*program, fShader);

  glBindAttribLocation(*program, 0, "inputPosition");
  glBindAttribLocation(*program, 1, "inputTexCoord");

  glLinkProgram(*program);
#if DEBUG_SDL
  int status;
  glGetProgramiv(*program, GL_LINK_STATUS, &status);
  if (status != 1)
  {
    int logSize;

    glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &logSize);
    logSize++;

    char infoLog[logSize];
    infoLog[logSize - 1] = '\0';

    glGetProgramInfoLog(*program, logSize, NULL, infoLog);
    printf("Failed to link program '%d'\n", *program);
    printf("%s\n", infoLog);
    exit(1);
  }
#endif
}

void renderQuad(GLuint program, GLuint vertexArrayId, struct Image image)
{
  glUseProgram(program);
  glBindVertexArray(vertexArrayId);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  glGenerateMipmap(GL_TEXTURE_2D);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void renderUnfilledQuad(GLuint program, GLuint vertexArrayId, struct Vec3f32 color)
{
  glUseProgram(program);
  glBindVertexArray(vertexArrayId);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

SDL_Window* initSDLWindow(SDL_GLContext* context, int screenWidth, int screenHeight)
{
  SDL_Window* window;

  SDL_Init(SDL_INIT_VIDEO);
  window   = SDL_CreateWindow("client", 0, 0, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
  *context = SDL_GL_CreateContext(window);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#if VSYNC
  SDL_GL_SetSwapInterval(1);
#else
  SDL_GL_SetSwapInterval(0);
#endif

  loadExtensions();

  return window;
}

void TurnZBufferOn()
{
  // Enable depth testing.
  glEnable(GL_DEPTH_TEST);
}
void TurnZBufferOff()
{
  // Disable depth testing.
  glDisable(GL_DEPTH_TEST);
}

void EnableAlphaBlending()
{
  // Enable alpha blending.
  glEnable(GL_BLEND);

  // Set the blending equation.
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}
void DisableAlphaBlending()
{
  // Disable alpha blending.
  glDisable(GL_BLEND);
}
