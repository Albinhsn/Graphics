#include "lodepng.h"
#include "mesh.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

int main() {

  unsigned char *image = NULL;
  unsigned int w, h;
  const char *filename = "./data/base-map.png";

  unsigned int error =
      lodepng_decode_file(&image, &w, &h, filename, LCT_RGBA, 8);

  int screenWidth = w;
  int screenHeight = h;
  SDL_Window *window;
  SDL_GLContext context;

  struct Mesh mesh;
  CreateQuadMesh(&mesh);

  window = SDL_CreateWindow("client", 0, 0, screenWidth, screenHeight,
                            SDL_WINDOW_OPENGL);
  context = SDL_GL_CreateContext(window);

  GLuint program, vShader, fShader, vertexArrayId, vertexBufferId;

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  int result = gladLoadGLLoader(SDL_GL_GetProcAddress);

  vShader = glad_glCreateShader(GL_VERTEX_SHADER);
  glad_glShaderSource(vShader, 1, &vertexSource, NULL);
  glad_glCompileShader(vShader);

  fShader = glad_glCreateShader(GL_FRAGMENT_SHADER);
  glad_glShaderSource(fShader, 1, &fragmentSource, NULL);
  glad_glCompileShader(fShader);

  program = glad_glCreateProgram();
  glad_glAttachShader(program, vShader);
  glad_glAttachShader(program, fShader);
  glad_glLinkProgram(program);

  bool running = true;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
        running = false;
      }
    }
  }

  SDL_Quit();
  return 0;
}
