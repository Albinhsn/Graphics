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

int main()
{

  srand(0);
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

  struct WavefrontObject obj;
  initWavefront(&obj);
  parseWavefrontObject(&obj, "./diablo3_pose/diablo3_pose.obj");

  i32*           zBuffer          = (i32*)malloc(sizeof(i32) * WIDTH * HEIGHT);
  u8*            shadowBufferData = (u8*)malloc(sizeof(u8) * WIDTH * HEIGHT * 4);
  i32*           zTextureBuffer   = (i32*)malloc(sizeof(i32) * WIDTH * HEIGHT);
  for(i32 i = 0; i < WIDTH * HEIGHT; i++){
    zTextureBuffer[i] = 0;
  }

  struct Vec3f32 center           = {0.0f, 0.0f, 0.0f};

  struct Image   shadowBuffer;
  initImage(&shadowBuffer, WIDTH, HEIGHT, shadowBufferData);

  struct Matrix4x4 projectionMatrix;
  buildIdentityMatrix4x4(&projectionMatrix);

  struct Matrix4x4 viewport;
  buildIdentityMatrix4x4(&viewport);
  buildViewportMatrix4x4(&viewport, WIDTH / 8, HEIGHT / 8, (WIDTH * 3) / 4, (HEIGHT * 3) / 4, DEPTH);

  i32            totalSims = 100;

  struct Vec3f32 up        = UP;

  for (i32 sims = 0; sims < totalSims; sims++)
  {
    for (i32 i = 0; i < WIDTH * HEIGHT; i++)
    {
      shadowBufferData[i * 4 + 0] = 0;
      shadowBufferData[i * 4 + 1] = 0;
      shadowBufferData[i * 4 + 2] = 0;
      shadowBufferData[i * 4 + 3] = 255;
    }
    printf("Sim: %d\n", sims);
    // Should be randomized
    struct Vec3f32 eye = randomPointOnSphere();
    eye.y = eye.y < 0 ? -eye.y : eye.y;

    up.x               = (f32)rand() / (f32)RAND_MAX;
    up.y               = (f32)rand() / (f32)RAND_MAX;
    up.z               = (f32)rand() / (f32)RAND_MAX;

    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
      zBuffer[i] = -INT_MAX;
    }

    struct Matrix4x4 modelView          = lookAt(eye, center, up);
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

      fillShadowBuffer(&shadowBuffer, v0, v1, v2, viewport, projectionMatrix, modelView, center, zBuffer);
    }

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


      fillZTexture(&shadowBuffer, zBuffer, zTextureBuffer, v0, v1, v2, t0, t1, t2, viewport, projectionMatrix, modelView);
    }
  }

  free(zBuffer);
  struct Image image;
  u8           imageData[WIDTH * HEIGHT * 4];
  initImage(&image, WIDTH, HEIGHT, imageData);

  for (i32 i = 0; i < shadowBuffer.width * shadowBuffer.height; i++)
  {

    i32 res = (f32)zTextureBuffer[i] / ((f32)totalSims / 2.0f);
    if (res > 255)
    {
      res = 255;
    }
    image.data[i * 4 + 0] = res;
    image.data[i * 4 + 1] = res;
    image.data[i * 4 + 2] = res;
    image.data[i * 4 + 3] = 255;
  }

  saveTarga(&shadowBuffer, "shadowbuffer.tga");
  saveTarga(&image, "output2.tga");

  destroyWavefront(&obj);
  return 0;
}
