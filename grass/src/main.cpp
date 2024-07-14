
#include "common.h"
#include "input.h"
#include "renderer.h"
#include "shader.h"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_DEFINE_MATH_OPERATORS
#include "common.cpp"

#include "sdl.cpp"
#include "shader.cpp"
#include "vector.cpp"

#include "../libs/imgui/backends/imgui_impl_opengl3.cpp"
#include "../libs/imgui/backends/imgui_impl_sdl2.cpp"
#include "../libs/imgui/imgui.cpp"
#include "../libs/imgui/imgui_draw.cpp"
#include "../libs/imgui/imgui_tables.cpp"
#include "../libs/imgui/imgui_widgets.cpp"

#include "files.cpp"
#include "input.cpp"
#include "renderer.cpp"

void init_imgui(SDL_Window *window, SDL_GLContext context) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui_ImplSDL2_InitForOpenGL(window, context);
  ImGui_ImplOpenGL3_Init();
}

u32 create_ground_buffer(Renderer *renderer) {
  BufferAttributes attributes[3] = {
      (BufferAttributes){3, BUFFER_ATTRIBUTE_FLOAT},
  };

  f32 buffer_data[12] = {
      1.0f,  1.0f,  0, //
      1.0f,  -1.0,  0, //
      -1.0f, -1.0f, 0, //
      -1.0f, 1.0,   0  //

  };
  u32 indices[6] = {1, 3, 0, 1, 2, 3};

  return renderer->create_buffer_indices(
      ArrayCount(buffer_data) * sizeof(f32), buffer_data, ArrayCount(indices),
      indices, attributes, ArrayCount(attributes));
}

int main() {
  u32 screen_width = 1024, screen_height = 720;
  Renderer renderer = Renderer(screen_width, screen_height, true);
  InputState input_state = InputState(renderer.window);

  init_imgui(renderer.window, renderer.context);

  ShaderType ground_shader_types[2] = {SHADER_TYPE_VERT, SHADER_TYPE_FRAG};
  const char *ground_shader_locations[2] = {"./shaders/ground.vs",
                                            "./shaders/ground.fs"};
  Shader ground_shader =
      Shader(ground_shader_types, ground_shader_locations, 2, "ground");
    ground_shader.use();

  u32 ground_buffer = create_ground_buffer(&renderer);

  Mat44 m = Mat44::identity();

  ground_shader.set_mat4("model", m);
  ground_shader.set_mat4("projection", m);

  u32 ticks = 0;
  while (true) {

    input_state.update();
    if (input_state.should_quit()) {
      break;
    }
    renderer.clear_framebuffer();

    renderer.render_buffer(ground_buffer);

    renderer.swap_buffers();
  }
}
