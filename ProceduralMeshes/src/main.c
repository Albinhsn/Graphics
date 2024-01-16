#include "files.h"
#include "mesh.h"
#include "sdl.h"
#include "string.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

int main() {
  int screenWidth = 620;
  int screenHeight = 480;

  const char *filename = "./data/base-map.tga";

  struct Image *image = LoadTarga(filename);

  struct String *vertexSource = ah_ReadFile("shaders/texture.vs");
  const char *constVertexSource = (const char *)vertexSource->buffer;
  struct String *fragmentSource = ah_ReadFile("shaders/texture.ps");
  const char *constFragmentSource = (const char *)fragmentSource->buffer;

  SDL_Window *window;
  SDL_GLContext context;

  struct Mesh mesh;
  CreateQuadMesh(&mesh);

  window = SDL_CreateWindow("client", 0, 0, screenWidth, screenHeight,
                            SDL_WINDOW_OPENGL);
  context = SDL_GL_CreateContext(window);

  GLuint program, vShader, fShader, vertexArrayId, vertexBufferId;

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &constVertexSource, NULL);
  glCompileShader(vShader);

  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &constFragmentSource, NULL);
  glCompileShader(fShader);

  program = glCreateProgram();
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);
  glLinkProgram(program);
  glUseProgram(program);

  bool running = true;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
        running = false;
      }
      glClearColor(1.0f, 0.753f, 0.796f, 1.0f);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

      // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      SDL_GL_SwapWindow(window);
    }
  }

  SDL_Quit();
  return 0;
}
