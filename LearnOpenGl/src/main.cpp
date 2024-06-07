
#include "common.h"
#include "files.h"
#include "sdl.h"
#include "shader.h"
#include "vector.h"
#include <SDL2/SDL_timer.h>

bool process_input()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
    {
      return true;
    }
  }
  return false;
}

int main()
{

  TargaImage image  = {};
  bool       result = sta_targa_read_from_file(&image, "./Assets/wall.tga");
  if (!result)
  {
    printf("Failed to read wall assets!\n");
    return 1;
  }

  TargaImage image2 = {};
  result            = sta_targa_read_from_file(&image2, "./Assets/awesomeface.tga");
  if (!result)
  {
    printf("Failed to read awesomeface!\n");
    return 1;
  }

  const int     screen_width  = 620;
  const int     screen_height = 480;
  SDL_Window*   window;
  SDL_GLContext context;
  sta_init_sdl_gl(&window, &context, screen_width, screen_height);

  float  vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,
                       0.5f,  -0.5f, 1.0f,  1.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,

                       -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
                       0.5f,  0.5f,  1.0f,  1.0f, -0.5f, 0.5f,  0.5f,  0.0f,  1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,

                       -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f,
                       -0.5f, -0.5f, 0.0f,  1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f, -0.5f, 0.5f,  0.5f,  1.0f,  0.0f,

                       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,
                       -0.5f, -0.5f, 0.0f,  1.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

                       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,
                       -0.5f, 0.5f,  1.0f,  0.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f, -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,

                       -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
                       0.5f,  0.5f,  1.0f,  0.0f, -0.5f, 0.5f,  0.5f,  0.0f,  0.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f

  };

  GLuint vao, vbo;
  sta_glGenVertexArrays(1, &vao);
  sta_glGenBuffers(1, &vbo);

  sta_glBindVertexArray(vao);
  sta_glBindBuffer(GL_ARRAY_BUFFER, vbo);
  sta_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ArrayCount(vertices), vertices, GL_STATIC_DRAW);

  sta_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  sta_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  sta_glEnableVertexAttribArray(0);
  sta_glEnableVertexAttribArray(1);

  unsigned int texture0, texture1;
  sta_glGenTextures(1, &texture0);
  sta_glBindTexture(GL_TEXTURE_2D, texture0);

  sta_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
  sta_glGenerateMipmap(GL_TEXTURE_2D);

  sta_glGenTextures(1, &texture1);
  sta_glBindTexture(GL_TEXTURE_2D, texture1);
  sta_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image2.width, image2.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.data);
  sta_glGenerateMipmap(GL_TEXTURE_2D);

  Shader shader("./shaders/tri.vert", "./shaders/tri.frag");

  Mat44  proj = {};
  proj.identity();
  proj.perspective(45.0f, screen_width / (f32)screen_height, 0.1f, 100.0f);

  Mat44 view = {};
  view.identity();
  view                    = view.translate(Vector3(0, 0, -3.0f));

  Mat44 look_at           = Mat44::look_at(Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1));

  view                    = look_at.mul(view);

  u32     ticks           = 0;
  Vector3 cubePositions[] = {Vector3(0.0f, 0.0f, 0.0f),   Vector3(2.0f, 5.0f, -15.0f), Vector3(-1.5f, -2.2f, -2.5f), Vector3(-3.8f, -2.0f, -12.3f), Vector3(2.4f, -0.4f, -3.5f),
                             Vector3(-1.7f, 3.0f, -7.5f), Vector3(1.3f, -2.0f, -2.5f), Vector3(1.5f, 2.0f, -2.5f),   Vector3(1.5f, 0.2f, -1.5f),    Vector3(-1.3f, 1.0f, -1.5f)};
  while (true)
  {
    if (process_input())
    {
      break;
    }
    if (ticks + 16 < SDL_GetTicks())
    {
      ticks = SDL_GetTicks() + 16;
      view  = view.translate(Vector3(0, 0, 3.0f));
      view  = view.rotate_y(0.5f);
      view  = view.translate(Vector3(0, 0, -3.0f));
    }

    sta_gl_clear_buffer(0, 0, 0, 1);
    shader.use();
    shader.set_int("texture1", 0);
    shader.set_int("texture2", 1);

    glActiveTexture(GL_TEXTURE0);
    sta_glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    sta_glBindTexture(GL_TEXTURE_2D, texture1);
    sta_glBindVertexArray(vao);

    // Mat44 view_t = view.transpose();
    // Mat44 proj_t = proj.transpose();
    shader.set_mat4("view", &view.m[0]);
    shader.set_mat4("projection", &proj.m[0]);

    for (unsigned int i = 0; i < ArrayCount(cubePositions); i++)
    {
      Mat44 trans = {};
      trans.identity();
      trans = trans.translate(cubePositions[i]);
      // Mat44 trans_t = trans.transpose();
      shader.set_mat4("model", &trans.m[0]);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    sta_gl_render(window);
  }
}
