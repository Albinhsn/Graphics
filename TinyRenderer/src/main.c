#include "common.h"
#include "files.h"
#include "image.h"
#include "vector.h"
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIEWSPACE_TO_WORLDSPACEY(x) (((x) + 1.0f) * (HEIGHT / 2.0f))
#define VIEWSPACE_TO_WORLDSPACEX(x) (((x) + 1.0f) * (WIDTH / 2.0f))

#define DEPTH 255

int main() {
  struct Vec3i32 lightDir = {0, 0, -1};
  ui8 data[WIDTH * HEIGHT * 4];
  i32 zBuffer[WIDTH * HEIGHT];
  for (int i = 0; i < WIDTH * HEIGHT; i++) {
    zBuffer[i] = INT_MIN;
  }

  struct Matrix4x4 projectionMatrix;
  buildIdentityMatrix4x4(&projectionMatrix);

  struct Matrix4x4 viewport;
  buildIdentityMatrix4x4(&viewport);
  buildViewportMatrix4x4(&viewport, WIDTH / 8, HEIGHT / 8, (WIDTH * 3) / 4,
                         (HEIGHT * 3) / 4, DEPTH);
  projectionMatrix.m[3][2] = -1.0f / 3.0f;

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

    struct Vec3f32 t0uvw =
        obj.textureCoordinates[face.verticesData[0].textureIdx - 1];
    struct Vec2f32 t0 = {t0uvw.x, t0uvw.y};
    struct Vec3f32 t1uvw =
        obj.textureCoordinates[face.verticesData[1].textureIdx - 1];
    struct Vec2f32 t1 = {t1uvw.x, t1uvw.y};
    struct Vec3f32 t2uvw =
        obj.textureCoordinates[face.verticesData[2].textureIdx - 1];
    struct Vec2f32 t2 = {t2uvw.x, t2uvw.y};

    struct Vec3i32 v0i32 = CREATE_VEC3i32(VIEWSPACE_TO_WORLDSPACEX(v0.x),
                                          VIEWSPACE_TO_WORLDSPACEY(v0.y),
                                          VIEWSPACE_TO_WORLDSPACEY(v0.z));
    struct Vec3i32 v1i32 = CREATE_VEC3i32(VIEWSPACE_TO_WORLDSPACEX(v1.x),
                                          VIEWSPACE_TO_WORLDSPACEY(v1.y),
                                          VIEWSPACE_TO_WORLDSPACEY(v1.z));
    struct Vec3i32 v2i32 = CREATE_VEC3i32(VIEWSPACE_TO_WORLDSPACEX(v2.x),
                                          VIEWSPACE_TO_WORLDSPACEY(v2.y),
                                          VIEWSPACE_TO_WORLDSPACEY(v2.z));
    struct Vec3i32 v0Proj = MatrixToVec3f32(MatMul4x4(
        viewport, MatMul4x4(projectionMatrix, Vec3f32ToMatrix(v0))));

    struct Vec3i32 v1Proj = MatrixToVec3f32(MatMul4x4(
        viewport, MatMul4x4(projectionMatrix, Vec3f32ToMatrix(v1))));
    struct Vec3i32 v2Proj = MatrixToVec3f32(MatMul4x4(
        viewport, MatMul4x4(projectionMatrix, Vec3f32ToMatrix(v2))));

    struct Vec3f32 normal =
        crossProduct3D(vectorSubtraction(CREATE_VEC3f32(v2.x, v2.y, v2.z),
                                         CREATE_VEC3f32(v0.x, v0.y, v0.z)),
                       vectorSubtraction(CREATE_VEC3f32(v1.x, v1.y, v1.z),
                                         CREATE_VEC3f32(v0.x, v0.y, v0.z)));
    normalizeVec3(&normal);
    f32 intensity = dotProductVec3(
        normal, CREATE_VEC3f32(lightDir.x, lightDir.y, lightDir.z));

    if (intensity > 0) {
      fillTriangle(&image, &texture, v0Proj, v1Proj, v2Proj, t0, t1, t2,
                   zBuffer);
    }
  }

  saveTarga(&image, "output.tga");
  saveTarga(&texture, "texture.tga");
  // destroyWavefront(&obj);
  return 0;
}
