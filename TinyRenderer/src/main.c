#include "common.h"
#include "files.h"
#include "image.h"
#include "lodepng.h"
#include "vector.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define VIEWSPACE_TO_WORLDSPACEY(x) (((x) + 1.0f) * (HEIGHT / 2.0f))
#define VIEWSPACE_TO_WORLDSPACEX(x) (((x) + 1.0f) * (WIDTH / 2.0f))

int main()
{

  bool         res;
  struct Image texture;
  res = loadTarga(&texture, "test.tga");
  if (!res)
  {
    return 0;
  }

  struct Image normalMap;
  res = loadTarga(&normalMap, "./diablo3_pose/diablo3_pose_nm_tangent.tga");
  if (!res)
  {
    return 0;
  }

  struct WavefrontObject obj;
  initWavefront(&obj);
  parseWavefrontObject(&obj, "./diablo3_pose/diablo3_pose.obj");

  struct Vec3f32 center = {0.0f, 0.0f, 0.0f};

  struct Image   image;
  i32            zBuffer[WIDTH * HEIGHT];
  u8             imageBuffer[WIDTH * HEIGHT * 4];
  initImage(&image, WIDTH, HEIGHT, imageBuffer);

  struct Matrix4x4 projectionMatrix;
  buildIdentityMatrix4x4(&projectionMatrix);

  struct Matrix4x4 viewport;
  buildIdentityMatrix4x4(&viewport);
  buildViewportMatrix4x4(&viewport, WIDTH / 8, HEIGHT / 8, (WIDTH * 3) / 4, (HEIGHT * 3) / 4, DEPTH);

  struct Vec3f32 eye = {0, 0, 1};

  for (int i = 0; i < WIDTH * HEIGHT; i++)
  {
    zBuffer[i] = -INT_MAX;
  }

  struct Matrix4x4 modelView          = lookAt(eye, center, UP);
  struct Vec4f32*  vertices           = obj.vertices;
  struct Vec3f32*  textureCoordinates = obj.textureCoordinates;
  struct Vec3f32*  normals            = obj.normals;

  for (i32 i = 0; i < obj.faceCount; i++)
  {
    struct WavefrontFace face        = obj.faces[i];

    struct VertexData    faceVertex0 = face.verticesData[0];
    struct VertexData    faceVertex1 = face.verticesData[1];
    struct VertexData    faceVertex2 = face.verticesData[2];

    struct Vec3f32       v0          = CAST_VEC4f32_TO_VEC3f32(vertices[faceVertex0.vertexIdx - 1]);
    struct Vec3f32       v1          = CAST_VEC4f32_TO_VEC3f32(vertices[faceVertex1.vertexIdx - 1]);
    struct Vec3f32       v2          = CAST_VEC4f32_TO_VEC3f32(vertices[faceVertex2.vertexIdx - 1]);

    struct Vec2f32       t0          = CAST_VEC3f32_TO_VEC2f32(textureCoordinates[faceVertex0.textureIdx - 1]);
    struct Vec2f32       t1          = CAST_VEC3f32_TO_VEC2f32(textureCoordinates[faceVertex1.textureIdx - 1]);
    struct Vec2f32       t2          = CAST_VEC3f32_TO_VEC2f32(textureCoordinates[faceVertex2.textureIdx - 1]);

    struct Vec3f32       n0          = normals[faceVertex0.normalIdx - 1];
    struct Vec3f32       n1          = normals[faceVertex1.normalIdx - 1];
    struct Vec3f32       n2          = normals[faceVertex2.normalIdx - 1];

    u8                   shadowBuffer[1];
    fillTriangle(&image, &texture, &normalMap, v0, v1, v2, t0, t1, t2, n0, n1, n2, zBuffer, viewport, projectionMatrix, modelView, shadowBuffer, modelView);
  }

  saveTarga(&image, "output2.tga");

  destroyWavefront(&obj);
  return 0;
}
