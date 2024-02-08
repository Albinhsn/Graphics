#include "common.h"
#include "files.h"
#include "image.h"
#include "vector.h"
#include <limits.h>

#define VIEWSPACE_TO_WORLDSPACEY(x) (((x) + 1.0f) * (HEIGHT / 2.0f))
#define VIEWSPACE_TO_WORLDSPACEX(x) (((x) + 1.0f) * (WIDTH / 2.0f))

#define DEPTH                       255

int main()
{
  struct Vec3i32 lightDir = {0, 0, -1};
  ui8            data[WIDTH * HEIGHT * 4];
  i32            zBuffer[WIDTH * HEIGHT];
  for (int i = 0; i < WIDTH * HEIGHT; i++)
  {
    zBuffer[i] = INT_MIN;
  }

  struct Matrix4x4 projectionMatrix;
  buildIdentityMatrix4x4(&projectionMatrix);

  struct Matrix4x4 viewport;
  buildIdentityMatrix4x4(&viewport);
  buildViewportMatrix4x4(&viewport, WIDTH / 8, HEIGHT / 8, (WIDTH * 3) / 4, (HEIGHT * 3) / 4, DEPTH);
  projectionMatrix.m[3][2] = -1.0f / 3.0f;

  struct Image image;
  initImage(&image, WIDTH, HEIGHT, data);

  struct Image texture;
  loadTarga(&texture, "./data/african_head_diffuse.tga");

  struct WavefrontObject obj;
  initWavefront(&obj);
  parseWavefrontObject(&obj, "./data/african_head.obj");

  for (i32 i = 0; i < obj.faceCount; i++)
  {
    struct Vec4f32*      vertices           = obj.vertices;
    struct Vec3f32*      textureCoordinates = obj.textureCoordinates;
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

    struct Vec3f32       normal             = crossProduct3D(vectorSubtraction(v2, v0), vectorSubtraction(v1, v0));
    normalizeVec3(&normal);

    f32 intensity = dotProductVec3(normal, CREATE_VEC3f32(lightDir.x, lightDir.y, lightDir.z));

    if (intensity > 0)
    {
      struct Vec3i32 v0Proj = MatrixToVec3f32(MatMul4x4(viewport, MatMul4x4(projectionMatrix, Vec3f32ToMatrix(v0))));
      struct Vec3i32 v1Proj = MatrixToVec3f32(MatMul4x4(viewport, MatMul4x4(projectionMatrix, Vec3f32ToMatrix(v1))));
      struct Vec3i32 v2Proj = MatrixToVec3f32(MatMul4x4(viewport, MatMul4x4(projectionMatrix, Vec3f32ToMatrix(v2))));

      fillTriangle(&image, &texture, v0Proj, v1Proj, v2Proj, t0, t1, t2, zBuffer);
    }
  }

  saveTarga(&image, "output.tga");
  destroyWavefront(&obj);
  return 0;
}
