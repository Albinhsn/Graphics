#include "shader.h"
#include "common.h"
#include "sdl.h"
#include <GL/glext.h>
#include <cassert>

bool test_shader_compilation(unsigned int id) {
  int success;
  char info_log[512] = {};
  sta_glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    sta_glGetShaderInfoLog(id, 512, NULL, info_log);
    printf("%s\n", info_log);
    return false;
  }

  return true;
}

static GLint get_location(GLuint id, const char *name,
                          const char *shader_name) {

  GLint location = sta_glGetUniformLocation(id, name);
  if (location == -1) {
    printf("Couldn't find uniform '%s' in shader '%s' %d\n", name,
                 shader_name, id);
    assert(!"Couldn't find uniform!");
  }
  return location;
}

bool test_program_linking(unsigned int id) {
  int success;
  char info_log[512] = {};
  sta_glGetProgramiv(id, GL_LINK_STATUS, &success);

  if (!success) {
    sta_glGetProgramInfoLog(id, 512, NULL, info_log);
    printf("%s\n", info_log);
    return false;
  }

  return true;
}

void Shader::set_bool(const char *name, bool value) {
  sta_glUniform1i(get_location(this->id, name, this->name), (int)value);
}

void Shader::set_float4f(const char *name, float f[4]) {
  sta_glUniform4fv(get_location(this->id, name, this->name), 1, &f[0]);
}
void Shader::set_int(const char *name, int value) {
  sta_glUniform1i(get_location(this->id, name, this->name), (int)value);
}

void Shader::set_float(const char *name, float value) {
  sta_glUniform1f(get_location(this->id, name, this->name), value);
}
void Shader::set_vec3(const char *name, Vector3 v) {
  sta_glUniform3fv(get_location(this->id, name, this->name), 1, (float *)&v);
}

void Shader::set_mat4(const char *name, Mat44 m) {
  this->set_mat4(name, &m.m[0], 1);
}
void Shader::set_mat4(const char *name, Mat44 *v, int count) {
  this->set_mat4(name, (float *)&v[0].m[0], count);
}

void Shader::set_mat4(const char *name, float *v, int count) {

  sta_glUniformMatrix4fv(get_location(this->id, name, this->name), count,
                         GL_FALSE, v);
}

void Shader::use() { sta_glUseProgram(this->id); }

GLuint compile_shader(const char *path, GLint shader_type) {

  Buffer buffer = {};
  if (!sta_read_file(&buffer, path)) {
    printf("Failed to read shader from '%s'\n", path);
    return -1;
  }
  const char *vertex_content = buffer.buffer;
  unsigned int vertex = sta_glCreateShader(shader_type);
  sta_glShaderSource(vertex, 1, &vertex_content, 0);
  sta_glCompileShader(vertex);

  if (!test_shader_compilation(vertex)) {
    printf("Failed shader compilation! for '%s' and type %d\n", path,
                 shader_type);
    return -1;
  }
  return vertex;
}

inline GLuint get_gl_shader_type(ShaderType type) {
  switch (type) {
  case SHADER_TYPE_VERT: {
    return GL_VERTEX_SHADER;
  }
  case SHADER_TYPE_FRAG: {
    return GL_FRAGMENT_SHADER;
  }
  case SHADER_TYPE_TESS_CONTROL: {
    return GL_TESS_CONTROL_SHADER;
  }
  case SHADER_TYPE_GEOMETRY: {
    return GL_GEOMETRY_SHADER;
  }
  case SHADER_TYPE_TESS_EVALUATION: {
    return GL_TESS_EVALUATION_SHADER;
  }
  case SHADER_TYPE_COMPUTE: {
    return GL_COMPUTE_SHADER;
  }
  }
  printf("Unknown type %d\n", type);
  assert(!"Unknown shader type to gl conversion?");
}

bool recompile_shader(Shader *shader) {
  Shader s;
  s.name = shader->name;
  s.id = sta_glCreateProgram();
  memset(s.locations, 0, sizeof(const char *) * ArrayCount(s.locations));

  printf("Reloading shader '%s'\n", shader->name);
  for (u32 i = 0;
       i < ArrayCount(shader->locations) && shader->locations[i] != 0; i++) {
    s.locations[i] = shader->locations[i];
    s.types[i] = shader->types[i];
    GLuint shader_id =
        compile_shader(s.locations[i], get_gl_shader_type(s.types[i]));
    if ((i32)shader_id == -1) {
      printf("Failed to recompile shader '%s'\n", s.locations[i]);
      return false;
    }
    sta_glAttachShader(s.id, shader_id);
    printf("Compiled '%s' %d and attached it to %d\n", s.locations[i],
                s.types[i], s.id);
  }

  sta_glLinkProgram(s.id);
  if (!test_program_linking(s.id)) {
    printf("Failed to link program for shader '%s'\n", s.name);
    return false;
  }

  // ToDo why does this increase :)
  for (u32 i = 0;
       i < ArrayCount(shader->locations) && shader->locations[i] != 0; i++) {
    sta_glDetachShader(get_gl_shader_type(shader->types[i]),
                       shader->shader_ids[i]);
    sta_glDeleteShader(shader->shader_ids[i]);
  }

  sta_glDeleteProgram(shader->id);

  *shader = s;
  return true;
}

Shader::Shader(ShaderType *types, const char **names, u32 count,
               const char *name) {
  this->name = name;
  this->id = sta_glCreateProgram();
  memset(this->locations, 0,
         ArrayCount(this->locations) * sizeof(const char *));

  for (u32 i = 0; i < count; i++) {
    this->types[i] = types[i];
    this->locations[i] = names[i];

    GLuint shader_id = compile_shader(names[i], get_gl_shader_type(types[i]));
    this->shader_ids[i] = shader_id;
    if ((i32)shader_id == -1) {
      printf("Failed to compile shader '%s'\n", names[i]);
      assert(!"Failed to compile shader!");
    }
    printf("Compiled shader '%s' of type %d -> %d\n", names[i], types[i],
                get_gl_shader_type(types[i]));
    sta_glAttachShader(this->id, shader_id);
  }
  sta_glLinkProgram(this->id);
  test_program_linking(this->id);
}