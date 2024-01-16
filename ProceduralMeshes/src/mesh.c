#include "mesh.h"

void CreateQuadMesh(struct Mesh *mesh) {

  mesh->length = 3;
  mesh->vertices = (struct Vec3 *)malloc(sizeof(struct Vec3) * mesh->length);
  // zero
  mesh->vertices[0] = (struct Vec3){0.0f, 0.0f, 0.0f};
  // Right
  mesh->vertices[1] = (struct Vec3){1.0f, 0.0f, 0.0f};
  // Up
  mesh->vertices[2] = (struct Vec3){0.0f, 1.0f, 0.0f};

  mesh->normals = (struct Vec3 *)malloc(sizeof(struct Vec3) * mesh->length);
  // Back * 3
  mesh->normals[0] = (struct Vec3){0.0f, 0.0f, -1.0f};
  mesh->normals[1] = (struct Vec3){0.0f, 0.0f, -1.0f};
  mesh->normals[2] = (struct Vec3){0.0f, 0.0f, -1.0f};

  mesh->uv = (struct Vec2 *)malloc(sizeof(struct Vec2) * mesh->length);
  mesh->uv[0] = (struct Vec2){0.0f, 0.0f};
  mesh->uv[1] = (struct Vec2){1.0f, 0.0f};
  mesh->uv[2] = (struct Vec2){0.0f, 1.0f};

  mesh->tangents = (struct Vec4 *)malloc(sizeof(struct Vec4) * mesh->length);
  mesh->tangents[0] = (struct Vec4){1.0f, 0.0f, 0.0f, -1.0f};
  mesh->tangents[1] = (struct Vec4){1.0f, 0.0f, 0.0f, -1.0f};
  mesh->tangents[2] = (struct Vec4){1.0f, 0.0f, 0.0f, -1.0f};
}
