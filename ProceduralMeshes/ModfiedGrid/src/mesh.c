#include "mesh.h"
#include <stdio.h>

void CreateQuadMesh(struct Mesh *mesh, int meshSize) {

  mesh->bufferDatalength = 4 * meshSize * meshSize;
  mesh->bufferData = (struct BufferData2 *)malloc(sizeof(struct BufferData2) *
                                                  mesh->bufferDatalength);

  float minX = -0.4f, maxX = 0.4f;
  float minY = -0.8f, maxY = 0.8f;

  float uvX = 0.0f, uvY = 0.0f;

  float offsetX = (maxX - minX) / meshSize;
  float offsetY = (maxY - minY) / meshSize;

  // These are just dependent on the amount of meshes and their position
  mesh->indicesLength = 6 * (meshSize * meshSize);
  mesh->indices = (int *)malloc(sizeof(int) * mesh->indicesLength);
  int indexIdx;
  float x = minX - offsetX, y = minY;
  for (int meshIdx = 0; meshIdx < meshSize * meshSize; meshIdx++) {
    if (meshIdx % meshSize == 0) {
      x += offsetX;
      y = minY;
    } else {
      y += offsetY;
    }
    int bufferOffset = meshIdx * 4;
    mesh->bufferData[bufferOffset + 0].vertices = (struct Vec3){x, y, 0.0f};

    mesh->bufferData[bufferOffset + 0].normals =
        (struct Vec3){0.0f, 0.0f, -1.0f};
    mesh->bufferData[bufferOffset + 0].uv = (struct Vec2){0.0f, 1.0f};
    printf("%lf %lf %lf %lf %lf\n", x, y, 0.0f, 0.0f, 1.0f);
    mesh->bufferData[bufferOffset + 0].tangents =
        (struct Vec3){1.0f, 0.0f, 0.0f};
    mesh->bufferData[bufferOffset + 0].time = 0.0f;
    mesh->bufferData[bufferOffset + 0].binormal =
        (struct Vec3){0.0f, 0.0f, 0.0f};

    mesh->bufferData[bufferOffset + 1].vertices =
        (struct Vec3){x + offsetX, y, 0.0f};
    mesh->bufferData[bufferOffset + 1].normals =
        (struct Vec3){0.0f, 0.0f, -1.0f};
    mesh->bufferData[bufferOffset + 1].uv = (struct Vec2){1.0f, 1.0f};
    printf("%lf %lf %lf %lf %lf\n", x + offsetX, y, 0.0f, 1.0f, 1.0f);
    mesh->bufferData[bufferOffset + 1].tangents =
        (struct Vec3){1.0f, 0.0f, 0.0f};
    mesh->bufferData[bufferOffset + 1].time = 0.0f;

    mesh->bufferData[bufferOffset + 2].vertices =
        (struct Vec3){x, y + offsetY, 0.0f};
    mesh->bufferData[bufferOffset + 2].normals =
        (struct Vec3){0.0f, 0.0f, -1.0f};
    mesh->bufferData[bufferOffset + 2].uv = (struct Vec2){0.0f, 0.0f};
    printf("%lf %lf %lf %lf %lf\n", x, y + offsetY, 0.0f, 0.0f, 0.0f);
    mesh->bufferData[bufferOffset + 2].tangents =
        (struct Vec3){1.0f, 0.0f, 0.0f};
    mesh->bufferData[bufferOffset + 2].time = 0.0f;

    mesh->bufferData[bufferOffset + 3].vertices =
        (struct Vec3){x + offsetX, y + offsetY, 0.0f};
    mesh->bufferData[bufferOffset + 3].normals =
        (struct Vec3){0.0f, 0.0f, -1.0f};
    mesh->bufferData[bufferOffset + 3].uv = (struct Vec2){1.0f, 0.0f};
    printf("%lf %lf %lf %lf %lf\n", x + offsetX, y + offsetY, 0.0f, 1.0f, 0.0f);
    mesh->bufferData[bufferOffset + 3].tangents =
        (struct Vec3){1.0f, 0.0f, 0.0f};
    mesh->bufferData[bufferOffset + 3].time = 0.0f;

    indexIdx = (bufferOffset * 6) / 4;

    mesh->indices[indexIdx + 0] = bufferOffset + 0;
    mesh->indices[indexIdx + 1] = bufferOffset + 1;
    mesh->indices[indexIdx + 2] = bufferOffset + 2;
    mesh->indices[indexIdx + 3] = bufferOffset + 1;
    mesh->indices[indexIdx + 4] = bufferOffset + 3;
    mesh->indices[indexIdx + 5] = bufferOffset + 2;
  }
  for (int i = 0; i < mesh->indicesLength; i++) {
    printf("%d ", mesh->indices[i]);
  }
  printf("\n");
}
