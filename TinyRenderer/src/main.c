#include "common.h"
#include "files.h"
#include "image.h"
#include "vector.h"
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 1000
#define HEIGHT 1000

#define VIEWSPACE_TO_WORLDSPACEY(x) (((x) + 1.0f) * (HEIGHT / 2.0f))
#define VIEWSPACE_TO_WORLDSPACEX(x) (((x) + 1.0f) * (WIDTH / 2.0f))

#define COLORSIZE 7

int main() {
  struct Vec3i32 lightDir = {0, 0, -1};
  ui8 data[WIDTH * HEIGHT * 4];
  f32 zBuffer[WIDTH * HEIGHT];
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    zBuffer[i] = -FLT_MAX;
  }

  struct Image image;
  initImage(&image, WIDTH, HEIGHT, data);

  struct WavefrontObject obj;
  initWavefront(&obj);
  parseWavefrontObject(&obj, "./data/african_head.obj");

  srand(0);

  for (i32 i = 0; i < obj.faceCount; i++) {
    struct WavefrontFace face = obj.faces[i];
    struct Vec3f32 v0 =
        CAST_TO_VEC3(obj.vertices[face.verticesData[0].vertexIdx - 1]);
    struct Vec3f32 v1 =
        CAST_TO_VEC3(obj.vertices[face.verticesData[1].vertexIdx - 1]);
    struct Vec3f32 v2 =
        CAST_TO_VEC3(obj.vertices[face.verticesData[2].vertexIdx - 1]);

    struct Vec3f32 normal =
        crossProduct3D(vectorSubtraction(CREATE_VEC3f32(v2.x, v2.y, v2.z),
                                         CREATE_VEC3f32(v0.x, v0.y, v0.z)),
                       vectorSubtraction(CREATE_VEC3f32(v1.x, v1.y, v1.z),
                                         CREATE_VEC3f32(v0.x, v0.y, v0.z)));
    normalizeVec3(&normal);
    f32 intensity = dotProductVec3(
        normal, CREATE_VEC3f32(lightDir.x, lightDir.y, lightDir.z));
    struct Vec4ui8 color = {(ui8)(255.0 * intensity), (ui8)(255.0 * intensity),
                            (ui8)(255.0 * intensity), 255};
    if (intensity > 0) {
      fillTriangle(&image, v0, v1, v2, color, zBuffer);
    }
  }

  saveTarga(&image, "output.tga");

  destroyWavefront(&obj);
  return 0;
}
