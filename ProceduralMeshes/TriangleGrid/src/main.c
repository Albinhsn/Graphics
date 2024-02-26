#include "files.h"
#include "mesh.h"
#include "png.h"
#include "sdl.h"
#include "string.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <sys/time.h>

void initializeTexture(GLuint *textureId) {
  glActiveTexture(GL_TEXTURE);
  glGenTextures(1, textureId);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, *textureId);
}

int main() {
  int screenWidth = 620;
  int screenHeight = 480;

  const char *filename = "./data/base-map.png";

  struct Image image;
  parsePNG(&image, filename);

  struct String vertexSource;
  ah_ReadFile(&vertexSource, "shaders/texture.vs");
  const char *constVertexSource = (const char *)vertexSource.buffer;
  struct String fragmentSource;
  ah_ReadFile(&fragmentSource, "shaders/texture.ps");
  const char *constFragmentSource = (const char *)fragmentSource.buffer;

  SDL_Window *window;
  SDL_GLContext context;
  GLuint program, vShader, fShader, vertexArrayId, vertexBufferId,
      indexBufferId;

  struct Mesh mesh;
  int gridSize = 15;
  CreateQuadMesh(&mesh, gridSize);

  window = SDL_CreateWindow("client", 0, 0, screenWidth, screenHeight,
                            SDL_WINDOW_OPENGL);
  context = SDL_GL_CreateContext(window);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetSwapInterval(0);

  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &constVertexSource, NULL);
  glCompileShader(vShader);

  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &constFragmentSource, NULL);
  glCompileShader(fShader);
  int status;
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
  if (status != 1) {
    int logSize;

    glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &logSize);
    logSize++;

    char infoLog[logSize];
    infoLog[logSize - 1] = '\0';

    glGetShaderInfoLog(vShader, logSize, NULL, infoLog);
    printf("%s\n", infoLog);
    return 0;
  }

  glGenVertexArrays(1, &vertexArrayId);
  glBindVertexArray(vertexArrayId);

  glGenBuffers(1, &vertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBufferData(GL_ARRAY_BUFFER,
               mesh.bufferDatalength * sizeof(struct BufferData2),
               mesh.bufferData, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct BufferData2),
                        0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct BufferData2),
                        (signed char *)NULL + (3 * sizeof(GLfloat)));
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(struct BufferData2),
                        (signed char *)NULL + (12 * sizeof(GLfloat)));

  glGenBuffers(1, &indexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indicesLength * sizeof(GLuint),
               mesh.indices, GL_STATIC_DRAW);

  program = glCreateProgram();
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);

  glBindAttribLocation(program, 0, "inputPosition");
  glBindAttribLocation(program, 1, "inputTexCoord");
  glBindAttribLocation(program, 2, "inputTime");

  glBindVertexArray(vertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

  glLinkProgram(program);
  glUseProgram(program);

  bool running = true;
  SDL_Event event;

  glActiveTexture(0);
  GLuint textureId;
  initializeTexture(&textureId);

  glActiveTexture(1);
  GLuint normalId;
  initializeTexture(&normalId);

  struct Image normalImage;
  parsePNG(&normalImage, "./data/normal-map.png");

  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  long long startTick = current_time.tv_usec;

  float counter = 0;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
        running = false;
      }
    }
    gettimeofday(&current_time, NULL);
    for (int i = 0; i < mesh.bufferDatalength; i++) {
      mesh.bufferData[i].time =
          (float)(current_time.tv_usec - startTick) / 100000;
    }
    glBufferData(GL_ARRAY_BUFFER,
                 mesh.bufferDatalength * sizeof(struct BufferData2),
                 mesh.bufferData, GL_STATIC_DRAW);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(1);
    glBindTexture(GL_TEXTURE_2D, normalId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, normalImage.width,
                 normalImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 normalImage.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_NEAREST);

    glGenerateMipmap(GL_TEXTURE_2D);

    glDrawElements(GL_TRIANGLES, mesh.indicesLength, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
  }

  SDL_Quit();
  return 0;
}
