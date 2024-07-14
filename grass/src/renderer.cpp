#include "renderer.h"
#include "common.h"
#include "platform.h"
#include "sdl.h"
#include "shader.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL2/SDL_video.h>
#include <cassert>


void Renderer::enable_2d_rendering()
{
  glDisable(GL_DEPTH_TEST);
}
void Renderer::disable_2d_rendering()
{
  glEnable(GL_DEPTH_TEST);
}

void Renderer::toggle_wireframe_on()
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void Renderer::toggle_wireframe_off()
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void Renderer::change_viewport(u32 w, u32 h)
{
  glViewport(0, 0, w, h);
}
void Renderer::reset_viewport_to_screen_size()
{
  glViewport(0, 0, screen_width, screen_height);
}

void Renderer::change_screen_size(u32 screen_width, u32 screen_height)
{
  SDL_SetWindowSize(this->window, screen_width, screen_height);
  glViewport(0, 0, screen_width, screen_height);
}

void Renderer::init_line_buffer()
{
  sta_glGenVertexArrays(1, &this->line_vao);
  sta_glGenBuffers(1, &this->line_vbo);

  sta_glBindVertexArray(this->line_vao);
  sta_glBindBuffer(GL_ARRAY_BUFFER, this->line_vbo);
  const int vertices_in_a_line = 4;
  f32       tot[4]             = {
      1.0f, 1.0f, //
      1.0f, -1.0, //
  };
  sta_glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * vertices_in_a_line, tot, GL_DYNAMIC_DRAW);

  sta_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 2, (void*)(0));
  sta_glEnableVertexAttribArray(0);
}

void Renderer::draw_line(f32 x1, f32 y1, f32 x2, f32 y2, u32 line_width, Color color)
{
  this->enable_2d_rendering();
  f32    lines[4] = {x1, y1, x2, y2};
  Shader s        = *this->get_shader_by_index(this->get_shader_by_name("quad"));
  s.use();
  s.set_float4f("color", (float*)&color);

  sta_glBindVertexArray(this->line_vao);
  sta_glBindBuffer(GL_ARRAY_BUFFER, this->line_vbo);
  sta_glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 4, lines, GL_DYNAMIC_DRAW);
  glLineWidth(line_width);
  glDrawArrays(GL_LINES, 0, 2);
  this->disable_2d_rendering();
}


void Renderer::bind_cube_texture(Shader shader, const char* uniform_name, u32 texture_index)
{
  Texture texture = this->textures[texture_index];
  if (texture.unit == -1)
  {
    texture.unit = this->get_free_texture_unit();
  }

  shader.use();
  GLuint location = sta_glGetUniformLocation(shader.id, uniform_name);
  sta_glUniform1i(location, texture.unit);
  glActiveTexture(GL_TEXTURE0 + texture.unit);
  sta_glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);
}

void Renderer::bind_texture(Shader shader, const char* uniform_name, u32 texture_index)
{
  Texture texture = this->textures[texture_index];
  if (texture.unit == -1)
  {
    texture.unit = this->get_free_texture_unit();
  }

  shader.use();
  GLuint location = sta_glGetUniformLocation(shader.id, uniform_name);
  sta_glUniform1i(location, texture.unit);
  glActiveTexture(GL_TEXTURE0 + texture.unit);
  sta_glBindTexture(GL_TEXTURE_2D, texture.id);
}

u32 Renderer::add_texture(u32 texture_id)
{
  RESIZE_ARRAY(this->textures, Texture, texture_count, texture_capacity);
  u32      out     = texture_count++;
  Texture* texture = &this->textures[out];
  texture->id      = texture_id;
  texture->unit    = this->get_free_texture_unit();
  return out;
}

u32 Renderer::create_texture(u32 width, u32 height, void* data)
{

  u32 texture;
  sta_glGenTextures(1, &texture);
  sta_glBindTexture(GL_TEXTURE_2D, texture);

  sta_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  sta_glGenerateMipmap(GL_TEXTURE_2D);

  return texture;
}

void Renderer::swap_buffers()
{
  sta_gl_render(this->window);
}

u32 Renderer::create_buffer_from_model(Model* model, BufferAttributes* attributes, u32 attribute_count)
{
  return this->create_buffer_indices(model->vertex_data_size * model->vertex_count, model->vertex_data, model->index_count, model->indices, attributes, attribute_count);
}

void Renderer::toggle_vsync()
{
  this->vsync = !this->vsync;
  SDL_GL_SetSwapInterval(this->vsync);
}
u32 Renderer::get_free_texture_unit()
{
  const static u32 MAX_UNITS = 38;
  u64              inv       = ~this->used_texture_units;

  u64              index     = __builtin_ctzll(inv);
  if (index == MAX_UNITS)
  {
    Texture* texture = &textures[0];
    u32      out     = texture->unit;
    texture->unit    = -1;
    return out;
  }
  this->used_texture_units |= index == 0 ? 1 : (1UL << index);
  return index;
}

u32 Renderer::get_texture(const char* name)
{
  for (u32 i = 0; i < this->texture_count; i++)
  {
    if (this->textures[i].id != -1 && compare_strings(name, this->textures[i].name))
    {
      return i;
    }
  }
  assert(!"Didn't find texture!");
}
Shader* Renderer::get_shader_by_index(u32 index)
{
  return &this->shaders[index];
}

void Renderer::reload_shaders()
{
  for (u32 i = 0; i < shader_count; i++)
  {
    Shader* shader = &shaders[i];
    if (!recompile_shader(shader))
    {
      printf("Failed to recompile shader '%s'\n", shader->name);
    }
  }
}

u32 Renderer::get_shader_by_name(const char* name)
{
  for (u32 i = 0; i < this->shader_count; i++)
  {
    if (compare_strings(name, this->shaders[i].name))
    {
      return i;
    }
  }
  assert(!"Couldn't find shader!");
}

u32 Renderer::create_buffer(u64 buffer_size, void* buffer_data, BufferAttributes* attributes, u64 attribute_count)
{
  GLBufferIndex buffer = {};
  sta_glGenVertexArrays(1, &buffer.vao);
  sta_glGenBuffers(1, &buffer.vbo);

  sta_glBindVertexArray(buffer.vao);
  sta_glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
  sta_glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer_data, GL_STATIC_DRAW);

  u32 size = 0;
  for (u32 i = 0; i < attribute_count; i++)
  {
    size += attributes[i].count;
  }
  size *= 4;
  u32 stride = 0;
  for (u32 i = 0; i < attribute_count; i++)
  {

    BufferAttributes attribute = attributes[i];
    switch (attribute.type)
    {
    case BUFFER_ATTRIBUTE_FLOAT:
    {
      sta_glVertexAttribPointer(i, attribute.count, GL_FLOAT, GL_FALSE, size, (void*)(stride * sizeof(int)));
      break;
    }
    case BUFFER_ATTRIBUTE_INT:
    {
      sta_glVertexAttribIPointer(i, attribute.count, GL_INT, size, (void*)(stride * sizeof(int)));
      break;
    }
    }
    stride += attribute.count;
    sta_glEnableVertexAttribArray(i);
  }

  if (this->index_buffers_cap == 0)
  {
    this->index_buffers_count = 1;
    this->index_buffers_cap   = 1;
    this->index_buffers       = (GLBufferIndex*)sta_allocate_struct(GLBufferIndex, 1);
    this->index_buffers[0]    = buffer;
    return 0;
  }
  RESIZE_ARRAY(this->index_buffers, GLBufferIndex, this->index_buffers_count, this->index_buffers_cap);
  this->index_buffers[this->index_buffers_count] = buffer;
  return this->index_buffers_count++;
}

u32 Renderer::create_buffer_indices(u64 buffer_size, void* buffer_data, u64 index_count, u32* indices, BufferAttributes* attributes, u32 attribute_count)
{
  GLBufferIndex buffer = {};
  buffer.index_count   = index_count;
  sta_glGenVertexArrays(1, &buffer.vao);
  sta_glGenBuffers(1, &buffer.vbo);
  sta_glGenBuffers(1, &buffer.ebo);

  sta_glBindVertexArray(buffer.vao);
  sta_glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
  sta_glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer_data, GL_STATIC_DRAW);

  sta_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
  sta_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * index_count, indices, GL_STATIC_DRAW);

  u32 size = 0;
  for (u32 i = 0; i < attribute_count; i++)
  {
    size += attributes[i].count;
  }
  size *= 4;
  u32 stride = 0;
  for (u32 i = 0; i < attribute_count; i++)
  {

    BufferAttributes attribute = attributes[i];
    switch (attribute.type)
    {
    case BUFFER_ATTRIBUTE_FLOAT:
    {
      sta_glVertexAttribPointer(i, attribute.count, GL_FLOAT, GL_FALSE, size, (void*)(stride * sizeof(int)));
      break;
    }
    case BUFFER_ATTRIBUTE_INT:
    {
      sta_glVertexAttribIPointer(i, attribute.count, GL_INT, size, (void*)(stride * sizeof(int)));
      break;
    }
    }
    stride += attribute.count;
    sta_glEnableVertexAttribArray(i);
  }

  if (this->index_buffers_cap == 0)
  {
    this->index_buffers_count = 1;
    this->index_buffers_cap   = 1;
    this->index_buffers       = (GLBufferIndex*)sta_allocate_struct(GLBufferIndex, 1);
    this->index_buffers[0]    = buffer;
    return 0;
  }
  RESIZE_ARRAY(this->index_buffers, GLBufferIndex, this->index_buffers_count, this->index_buffers_cap);
  this->index_buffers[this->index_buffers_count] = buffer;
  return this->index_buffers_count++;
}
void Renderer::render_buffer(u32 buffer_id)
{
  sta_glBindVertexArray(this->index_buffers[buffer_id].vao);
  glDrawElements(GL_TRIANGLES, this->index_buffers[buffer_id].index_count, GL_UNSIGNED_INT, 0);
  sta_glBindVertexArray(0);
}

void Renderer::clear_framebuffer()
{
  sta_gl_clear_buffer(1, 1, 1, 1.0f);
}
