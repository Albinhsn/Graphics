#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

struct BufferData {
  struct Vec3 vertices;
  struct Vec2 uv;
  struct Vec3 normals;
  struct Vec4 tangents;
};

struct Mesh {

  int bufferDatalength;
  struct BufferData *bufferData;
  int *indices;
  int indicesLength;
};

void CreateQuadMesh(struct Mesh *mesh);

#endif
