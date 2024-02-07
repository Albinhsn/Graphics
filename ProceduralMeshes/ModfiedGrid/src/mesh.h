#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

struct BufferData2 {
  struct Vec3 vertices;
  struct Vec2 uv;
  float time;
  struct Vec3 normals;
  struct Vec3 tangents;
  struct Vec3 binormal;
};

struct BufferData {
  struct Vec3 vertices;
  struct Vec2 uv;
  struct Vec3 normals;
  struct Vec4 tangents;
};

struct Mesh {

  int bufferDatalength;
  struct BufferData2 *bufferData;
  int *indices;
  int indicesLength;
};

void CreateQuadMesh(struct Mesh *mesh, int meshSize);

#endif
