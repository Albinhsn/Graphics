
#include "common.h"
#include "files.h"
#include "sdl.h"
#include "shader.h"
#include "vector.h"

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

  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  GLuint vao, vbo, ebo;
  sta_glGenVertexArrays(1, &vao);
  sta_glGenBuffers(1, &vbo);
  sta_glGenBuffers(1, &ebo);

  sta_glBindVertexArray(vao);
  sta_glBindBuffer(GL_ARRAY_BUFFER, vbo);
  sta_glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ArrayCount(vertices), vertices, GL_STATIC_DRAW);

  sta_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  sta_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * ArrayCount(indices), indices, GL_STATIC_DRAW);

  sta_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  sta_glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
  sta_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  sta_glEnableVertexAttribArray(0);
  sta_glEnableVertexAttribArray(1);
  sta_glEnableVertexAttribArray(2);

  unsigned int texture0, texture1;
  sta_glGenTextures(1, &texture0);
  sta_glBindTexture(GL_TEXTURE_2D, texture0);

  sta_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
  sta_glGenerateMipmap(GL_TEXTURE_2D);

  sta_glGenTextures(1, &texture1);
  sta_glBindTexture(GL_TEXTURE_2D, texture1);
  sta_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image2.width, image2.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.data);
  sta_glGenerateMipmap(GL_TEXTURE_2D);

  Shader  shader("./shaders/tri.vert", "./shaders/tri.frag");

  Vector4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  Mat44   trans = {};
  trans.identity();
  trans            = trans.rotate_x(-55.0f);
  Mat44 view       = {};
  view             = view.translate(Vector3(0, 0, -3.0f));

  Mat44 projection = {};
  projection.perspective(45.0f, screen_width / (f32)screen_height, 0.1f, 100.0f);

  while (true)
  {
    if (process_input())
    {
      break;
    }

    shader.use();
    shader.set_int("texture1", 0);
    shader.set_int("texture2", 1);

    shader.set_mat4("model", &trans.m[0]);
    shader.set_mat4("view", &view.m[0]);
    shader.set_mat4("projection", &projection.m[0]);

    sta_gl_clear_buffer(0, 0, 0, 1);

    glActiveTexture(GL_TEXTURE0);
    sta_glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    sta_glBindTexture(GL_TEXTURE_2D, texture1);

    sta_glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    sta_gl_render(window);
  }
}
