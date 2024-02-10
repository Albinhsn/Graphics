#include "common.h"
#include "files.h"
#include "image.h"
#include "lodepng.h"
#include "vector.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define VIEWSPACE_TO_WORLDSPACEY(x) (((x) + 1.0f) * (HEIGHT / 2.0f))
#define VIEWSPACE_TO_WORLDSPACEX(x) (((x) + 1.0f) * (WIDTH / 2.0f))

#define DEPTH                       255

#define UP                          ((struct Vec3f32){0.0f, 1.0f, 0.0f})

int main()
{

  ui8            data[WIDTH * HEIGHT * 4];
  i32            zBuffer[WIDTH * HEIGHT];
  struct Vec3f32 eye    = {1.0f, 1.0f, 3.0f};
  struct Vec3f32 center = {0.0f, 0.0f, 0.0f};

  for (int i = 0; i < WIDTH * HEIGHT; i++)
  {
    zBuffer[i] = INT_MIN;
  }

  struct Matrix4x4 projectionMatrix;
  buildIdentityMatrix4x4(&projectionMatrix);

  struct Matrix4x4 viewport;
  buildIdentityMatrix4x4(&viewport);
  buildViewportMatrix4x4(&viewport, WIDTH / 8, HEIGHT / 8, (WIDTH * 3) / 4, (HEIGHT * 3) / 4, DEPTH);
  struct Vec3f32 eyeCenter   = {eye.x - center.x, eye.y - center.y, eye.z - center.z};
  f32            projScale   = sqrtf(dotProductVec3(eyeCenter, eyeCenter));

  projectionMatrix.m[3][2]   = -1.0f / projScale;

  struct Matrix4x4 modelView = lookAt(eye, center, UP);

  struct Image     image;
  initImage(&image, WIDTH, HEIGHT, data);

  bool         res;
  struct Image texture;
  res = loadTarga(&texture, "./diablo3_pose/diablo3_pose_diffuse.tga");
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

  // struct Image normalMapTangent;
  // loadTarga(&normalMapTangent, "./data/diablo3_pose_nm.tga");

  struct WavefrontObject obj;

  initWavefront(&obj);
  parseWavefrontObject(&obj, "./diablo3_pose/diablo3_pose.obj");

  for (i32 i = 0; i < obj.faceCount; i++)
  {
    struct Vec4f32*      vertices           = obj.vertices;
    struct Vec3f32*      textureCoordinates = obj.textureCoordinates;
    struct Vec3f32*      normals            = obj.normals;
    struct WavefrontFace face               = obj.faces[i];

    struct VertexData    faceVertex0        = face.verticesData[0];
    struct VertexData    faceVertex1        = face.verticesData[1];
    struct VertexData    faceVertex2        = face.verticesData[2];

    struct Vec3f32       v0                 = CAST_VEC4f32_TO_VEC3f32(vertices[faceVertex0.vertexIdx - 1]);
    struct Vec3f32       v1                 = CAST_VEC4f32_TO_VEC3f32(vertices[faceVertex1.vertexIdx - 1]);
    struct Vec3f32       v2                 = CAST_VEC4f32_TO_VEC3f32(vertices[faceVertex2.vertexIdx - 1]);

    struct Vec2f32       t0                 = CAST_VEC3f32_TO_VEC2f32(textureCoordinates[faceVertex0.textureIdx - 1]);
    struct Vec2f32       t1                 = CAST_VEC3f32_TO_VEC2f32(textureCoordinates[faceVertex1.textureIdx - 1]);
    struct Vec2f32       t2                 = CAST_VEC3f32_TO_VEC2f32(textureCoordinates[faceVertex2.textureIdx - 1]);

    struct Vec3f32       n0                 = normals[faceVertex0.normalIdx - 1];
    normalizeVec3(&n0);
    struct Vec3f32 n1                       = normals[faceVertex1.normalIdx - 1];
    normalizeVec3(&n1);
    struct Vec3f32 n2                       = normals[faceVertex2.normalIdx - 1];
    normalizeVec3(&n2);

    fillTriangle(&image, &texture, &normalMap, v0, v1, v2, t0, t1, t2, n0, n1, n2, zBuffer, viewport, projectionMatrix, modelView);
  }

  saveTarga(&image, "output2.tga");

  destroyWavefront(&obj);
  return 0;
}
