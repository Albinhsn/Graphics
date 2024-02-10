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

#define DEPTH                       255

#define UP                          ((struct Vec3f32){0.0f, 1.0f, 0.0f})

int main()
{
  ui8            data[WIDTH * HEIGHT * 4];
  i32            zBuffer[WIDTH * HEIGHT];
  struct Vec3f32 eye    = {0.0f, 0.0f, 1.0f};
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
  projectionMatrix.m[3][2]   = -1.0f / 3.0f;

  struct Matrix4x4 modelView = lookAt(eye, center, UP);

  struct Image     image;
  initImage(&image, WIDTH, HEIGHT, data);

  struct Image texture;
  loadTarga(&texture, "./data/african_head_diffuse.tga.tga");

  struct Image normalMap;
  loadTarga(&normalMap, "./data/african_head_nm_tangent.tga");

  // struct Image normalMapTangent;
  // loadTarga(&normalMapTangent, "./data/diablo3_pose_nm.tga");

  struct WavefrontObject obj;

  initWavefront(&obj);
  parseWavefrontObject(&obj, "./data/african_head.obj");

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
    struct Vec3f32       n1                 = normals[faceVertex1.normalIdx - 1];
    struct Vec3f32       n2                 = normals[faceVertex2.normalIdx - 1];

    struct Vec3f32       v0Projf32          = CAST_VEC4f32_TO_VEC3f32(MatVecMul4x4(projectionMatrix, MatVecMul4x4(modelView, CREATE_VEC4f32(v0.x, v0.y, v0.z, 1.0f))));
    struct Vec3f32       v1Projf32          = CAST_VEC4f32_TO_VEC3f32(MatVecMul4x4(projectionMatrix, MatVecMul4x4(modelView, CREATE_VEC4f32(v1.x, v1.y, v1.z, 1.0f))));
    struct Vec3f32       v2Projf32          = CAST_VEC4f32_TO_VEC3f32(MatVecMul4x4(projectionMatrix, MatVecMul4x4(modelView, CREATE_VEC4f32(v2.x, v2.y, v2.z, 1.0f))));

    struct Vec3i32       v0Proj             = MatrixToVec3f32(MatMul4x4(viewport, MatMul4x4(projectionMatrix, MatMul4x4(modelView, Vec3f32ToMatrix(v0)))));
    struct Vec3i32       v1Proj             = MatrixToVec3f32(MatMul4x4(viewport, MatMul4x4(projectionMatrix, MatMul4x4(modelView, Vec3f32ToMatrix(v1)))));
    struct Vec3i32       v2Proj             = MatrixToVec3f32(MatMul4x4(viewport, MatMul4x4(projectionMatrix, MatMul4x4(modelView, Vec3f32ToMatrix(v2)))));

    struct Matrix4x4     viewProjModel      = MatMul4x4(viewport, MatMul4x4(projectionMatrix, modelView));

    fillTriangle(&image, &texture, &normalMap, v0Proj, v1Proj, v2Proj, t0, t1, t2, n0, n1, n2, zBuffer, viewProjModel, v0Projf32, v1Projf32, v2Projf32);
  }

  saveTarga(&image, "output2.tga");

  destroyWavefront(&obj);
  return 0;
}
