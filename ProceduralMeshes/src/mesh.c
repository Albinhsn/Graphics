#include "mesh.h"

void CreateQuadMesh(struct Mesh *mesh, int meshSize) {

  mesh->bufferDatalength =  4 * meshSize;
  mesh->bufferData = (struct BufferData *)malloc(sizeof(struct BufferData) *
                                                 mesh->bufferDatalength);
  
  float minX = -0.4f, maxX = 0.4f;
  float minY = -0.8f, maxY = 0.8f;

  float offsetX = (maxX - minX) / meshSize;
  float offsetY = (maxY - minY) / meshSize;

  // These are just dependent on the amount of meshes and their position

  mesh->bufferData[0].vertices = (struct Vec3){-0.4f, -0.8f, 0.0f};
  mesh->bufferData[0].normals = (struct Vec3){0.0f, 0.0f, -1.0f};
  mesh->bufferData[0].uv = (struct Vec2){0.0f, 0.0f};
  mesh->bufferData[0].tangents = (struct Vec4){1.0f, 0.0f, 0.0f, -1.0f};

  mesh->bufferData[1].vertices = (struct Vec3){0.4f, -0.8f, 0.0f};
  mesh->bufferData[1].normals = (struct Vec3){0.0f, 0.0f, -1.0f};
  mesh->bufferData[1].uv = (struct Vec2){1.0f, 0.0f};
  mesh->bufferData[1].tangents = (struct Vec4){1.0f, 0.0f, 0.0f, -1.0f};

  mesh->bufferData[2].vertices = (struct Vec3){-0.4f, 0.8f, 0.0f};
  mesh->bufferData[2].normals = (struct Vec3){0.0f, 0.0f, -1.0f};
  mesh->bufferData[2].uv = (struct Vec2){0.0f, 1.0f};
  mesh->bufferData[2].tangents = (struct Vec4){1.0f, 0.0f, 0.0f, -1.0f};

  mesh->bufferData[3].vertices = (struct Vec3){0.4f, 0.8f, 0.0f};
  mesh->bufferData[3].normals = (struct Vec3){0.0f, 0.0f, -1.0f};
  mesh->bufferData[3].uv = (struct Vec2){1.0f, 1.0f};
  mesh->bufferData[3].tangents = (struct Vec4){1.0f, 0.0f, 0.0f, -1.0f};

  mesh->indicesLength = 6;
  mesh->indices = (int *)malloc(sizeof(int) * mesh->indicesLength);
  mesh->indices[0] = 0;
  mesh->indices[1] = 1;
  mesh->indices[2] = 2;
  mesh->indices[3] = 1;
  mesh->indices[4] = 3;
  mesh->indices[5] = 2;
}
