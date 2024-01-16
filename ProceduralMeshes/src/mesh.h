#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

struct Mesh {
  int length;
  struct Vec3 *vertices;
  struct Vec3 *normals;
  struct Vec4 *tangents;
  struct Vec2 *uv;
};

void CreateQuadMesh(struct Mesh *mesh);

#endif
