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

  struct Image texture;
  loadTarga(&texture, "./data/african_head_diffuse.tga");

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

    struct Vec3f32 t0 =
        obj.textureCoordinates[face.verticesData[0].textureIdx - 1];
    struct Vec3f32 t1 =
        obj.textureCoordinates[face.verticesData[1].textureIdx - 1];
    struct Vec3f32 t2 =
        obj.textureCoordinates[face.verticesData[2].textureIdx - 1];

    i32 t0u = t0.x * texture.width;
    i32 t0v = t0.y * texture.height;
    i32 t0Idx = (i32)(t0u + t0v * texture.width) * 4;

    struct Vec4ui8 c0 = {texture.data[t0Idx + 2], texture.data[t0Idx + 1],
                         texture.data[t0Idx + 0], 255};

    i32 t1u = t1.x * texture.width;
    i32 t1v = t1.y * texture.height;
    i32 t1Idx =
        (i32)(t1.x * texture.width + t1.y * texture.height * texture.width) * 4;
    struct Vec4ui8 c1 = {texture.data[t1Idx + 2], texture.data[t1Idx + 1],
                         texture.data[t1Idx + 0], 255};

    i32 t2u = t2.x * texture.width;
    i32 t2v = t2.y * texture.height;
    i32 t2Idx =
        (i32)(t2.x * texture.width + t2.y * texture.height * texture.width) * 4;
    struct Vec4ui8 c2 = {texture.data[t2Idx + 2], texture.data[t2Idx + 1],
                         texture.data[t2Idx + 0], 255};

    struct Vec3f32 normal =
        crossProduct3D(vectorSubtraction(CREATE_VEC3f32(v2.x, v2.y, v2.z),
                                         CREATE_VEC3f32(v0.x, v0.y, v0.z)),
                       vectorSubtraction(CREATE_VEC3f32(v1.x, v1.y, v1.z),
                                         CREATE_VEC3f32(v0.x, v0.y, v0.z)));
    normalizeVec3(&normal);
    f32 intensity = dotProductVec3(
        normal, CREATE_VEC3f32(lightDir.x, lightDir.y, lightDir.z));

    if (intensity > 0) {
      fillTriangle(&image, v0, v1, v2, c0, c1, c2, zBuffer);
    }
  }

  saveTarga(&image, "output.tga");
  saveTarga(&texture, "texture.tga");
  // destroyWavefront(&obj);
  return 0;
}
