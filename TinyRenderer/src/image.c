#include "image.h"
#include "common.h"
#include "vector.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

static inline void setPixel(struct Image* image, u16 x, u16 y, struct Vec4u8 color)
{
  i32 idxOffset              = (y * image->width + x) * 4;
  image->data[idxOffset + 0] = color.b;
  image->data[idxOffset + 1] = color.g;
  image->data[idxOffset + 2] = color.r;
  image->data[idxOffset + 3] = color.a;
}

static inline i16 absDiff(i16 x, i16 y)
{
  i16 diff = x - y;
  return diff < 0 ? -diff : diff;
}

static inline i16 absi16(i16 x)
{
  return x < 0 ? -x : x;
}

void setLine(struct Image* image, u16 x0, u16 y0, u16 x1, u16 y1, struct Vec4u8 color)
{

  bool steep = absi16(x0 - x1) < absi16(y0 - y1);

  if (steep)
  {
    i16 tmp = x0;
    x0      = y0;
    y0      = tmp;

    tmp     = x1;
    x1      = y1;
    y1      = tmp;
  }

  if (x0 > x1)
  {
    i16 tmp = x0;
    x0      = x1;
    x1      = tmp;

    tmp     = y0;
    y0      = y1;
    y1      = tmp;
  }

  i16  dx        = x1 - x0;
  i16  dy        = y1 - y0;

  i16  d         = 2 * dy;
  bool downwards = d < 0;
  d              = downwards ? -d : d;

  i16 derror     = 0;
  i16 y          = y0;
  for (i16 x = x0; x <= x1; x++)
  {
    setPixel(image, steep ? y : x, steep ? x : y, color);
    derror += d;
    if (derror > dx)
    {
      y = downwards ? y - 1 : y + 1;
      derror -= 2 * dx;
    }
  }
}

void initImage(struct Image* image, u16 width, u16 height, u8* data)
{

  image->width  = width;
  image->height = height;
  for (i32 i = 0; i < width * height * 4; i += 4)
  {
    data[i + 0] = 0;
    data[i + 1] = 0;
    data[i + 2] = 0;
    data[i + 3] = 0xFF;
  }

  image->data = data;
}

void debugImageData(struct Image* image)
{
  i32 pixelRowLength = image->width * 4;
  for (i32 y = 0; y < image->height; y++)
  {
    for (int x = 0; x < pixelRowLength; x += 4)
    {
      printf("|%d %d %d %d", image->data[y * pixelRowLength + x + 0], image->data[y * pixelRowLength + x + 1], image->data[y * pixelRowLength + x + 2], image->data[y * pixelRowLength + x + 3]);
    }
    printf("|\n");
  }
}

void fillZTexture(struct Image* shadowBuffer, i32* zBuffer, i32* zTexture, struct Vec3f32 v0, struct Vec3f32 v1, struct Vec3f32 v2, struct Vec2f32 uv0, struct Vec2f32 uv1, struct Vec2f32 uv2,
                  struct Matrix4x4 viewport, struct Matrix4x4 projectionMatrix, struct Matrix4x4 modelView)
{

  struct Matrix4x4 M         = MatMul4x4(viewport, modelView);

  struct Vec3f32   v0Proj    = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v0.x, v0.y, v0.z, 1.0f)));
  struct Vec3f32   v1Proj    = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v1.x, v1.y, v1.z, 1.0f)));
  struct Vec3f32   v2Proj    = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v2.x, v2.y, v2.z, 1.0f)));

  struct Vec4f32   glVertex0 = MatVecMul4x4(modelView, CREATE_VEC4f32(v0.x, v0.y, v0.z, 1.0f));
  struct Vec4f32   glVertex1 = MatVecMul4x4(modelView, CREATE_VEC4f32(v1.x, v1.y, v1.z, 1.0f));
  struct Vec4f32   glVertex2 = MatVecMul4x4(modelView, CREATE_VEC4f32(v2.x, v2.y, v2.z, 1.0f));

  i32              xMin      = ceil(MIN(MIN(v0Proj.x, v1Proj.x), v2Proj.x));
  i32              yMin      = ceil(MIN(MIN(v0Proj.y, v1Proj.y), v2Proj.y));

  i32              xMax      = floor(MAX(MAX(v0Proj.x, v1Proj.x), v2Proj.x));
  i32              yMax      = floor(MAX(MAX(v0Proj.y, v1Proj.y), v2Proj.y));

  f32              totalArea = crossProduct2Df32(CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(v0Proj.x, v0Proj.y));

  for (i32 x = xMin; x <= xMax; x++)
  {

    for (i32 y = yMin; y <= yMax; y++)
    {
      f32  w0     = crossProduct2Df32(CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(x, y));
      f32  w1     = crossProduct2Df32(CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(v0Proj.x, v0Proj.y), CREATE_VEC2f32(x, y));
      f32  w2     = crossProduct2Df32(CREATE_VEC2f32(v0Proj.x, v0Proj.y), CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(x, y));

      f32  alpha  = w0 / totalArea;
      f32  beta   = w1 / totalArea;
      f32  gamma  = w2 / totalArea;

      bool inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

      i32  z      = alpha * v0Proj.z + beta * v1Proj.z + gamma * v2Proj.z;

      if (inside && zBuffer[y * shadowBuffer->width + x] == z)
      {
        f32 u                                   = (alpha * uv0.x + beta * uv1.x + gamma * uv2.x);
        f32 v                                   = (alpha * uv0.y + beta * uv1.y + gamma * uv2.y);

        i32 tu                                  = u * shadowBuffer->width;
        i32 tv                                  = v * shadowBuffer->height;

        zTexture[tu + tv * shadowBuffer->width] = 255;
      }
    }
  }
}

void fillShadowBuffer(struct Image* shadowBuffer, struct Vec3f32 v0, struct Vec3f32 v1, struct Vec3f32 v2, struct Matrix4x4 viewport, struct Matrix4x4 projectionMatrix, struct Matrix4x4 modelView,
                      struct Vec3f32 center, i32* zBuffer)
{

  struct Matrix4x4 M         = MatMul4x4(viewport, modelView);

  struct Vec3f32   v0Proj    = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v0.x, v0.y, v0.z, 1.0f)));
  struct Vec3f32   v1Proj    = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v1.x, v1.y, v1.z, 1.0f)));
  struct Vec3f32   v2Proj    = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v2.x, v2.y, v2.z, 1.0f)));

  struct Vec4f32   glVertex0 = MatVecMul4x4(modelView, CREATE_VEC4f32(v0.x, v0.y, v0.z, 1.0f));
  struct Vec4f32   glVertex1 = MatVecMul4x4(modelView, CREATE_VEC4f32(v1.x, v1.y, v1.z, 1.0f));
  struct Vec4f32   glVertex2 = MatVecMul4x4(modelView, CREATE_VEC4f32(v2.x, v2.y, v2.z, 1.0f));

  i32              xMin      = ceil(MIN(MIN(v0Proj.x, v1Proj.x), v2Proj.x));
  i32              yMin      = ceil(MIN(MIN(v0Proj.y, v1Proj.y), v2Proj.y));

  i32              xMax      = floor(MAX(MAX(v0Proj.x, v1Proj.x), v2Proj.x));
  i32              yMax      = floor(MAX(MAX(v0Proj.y, v1Proj.y), v2Proj.y));

  f32              totalArea = crossProduct2Df32(CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(v0Proj.x, v0Proj.y));

  for (i32 x = xMin; x <= xMax; x++)
  {

    for (i32 y = yMin; y <= yMax; y++)
    {
      f32  w0     = crossProduct2Df32(CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(x, y));
      f32  w1     = crossProduct2Df32(CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(v0Proj.x, v0Proj.y), CREATE_VEC2f32(x, y));
      f32  w2     = crossProduct2Df32(CREATE_VEC2f32(v0Proj.x, v0Proj.y), CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(x, y));

      f32  alpha  = w0 / totalArea;
      f32  beta   = w1 / totalArea;
      f32  gamma  = w2 / totalArea;

      bool inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

      i32  z      = alpha * v0Proj.z + beta * v1Proj.z + gamma * v2Proj.z;

      if (inside && zBuffer[y * shadowBuffer->width + x] < z)
      {
        zBuffer[y * shadowBuffer->width + x] = z;

        f32 depth                            = glVertex0.z * alpha + glVertex1.z * beta + glVertex2.z * gamma;
        f32 diff                             = (depth + 1.0f) / 2.0f;
        diff                                 = diff > 1.0f ? 1.0f : diff;
        diff                                 = diff < 0.0f ? 0.0f : diff;

        struct Vec4u8 color                  = {255 * diff, 255 * diff, 255 * diff, 255};

        setPixel(shadowBuffer, x, y, color);
      }
    }
  }
}

void fillTriangle(struct Image* image, struct Image* texture, struct Image* normalMap, struct Vec3f32 v0, struct Vec3f32 v1, struct Vec3f32 v2, struct Vec2f32 uv0, struct Vec2f32 uv1,
                  struct Vec2f32 uv2, struct Vec3f32 n0, struct Vec3f32 n1, struct Vec3f32 n2, i32* zBuffer, struct Matrix4x4 viewport, struct Matrix4x4 projectionMatrix, struct Matrix4x4 modelView,
                  u8* shadowBuffer, struct Matrix4x4 shadowMatrix)
{

  struct Matrix4x4 projModel = MatMul4x4(projectionMatrix, modelView);
  struct Matrix4x4 M         = MatMul4x4(viewport, projModel);

  struct Vec3f32   v0Proj    = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v0.x, v0.y, v0.z, 1.0f)));
  struct Vec3f32   v1Proj    = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v1.x, v1.y, v1.z, 1.0f)));
  struct Vec3f32   v2Proj    = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v2.x, v2.y, v2.z, 1.0f)));

  struct Vec4f32   glVertex0 = MatVecMul4x4(projModel, CREATE_VEC4f32(v0.x, v0.y, v0.z, 1.0f));
  struct Vec4f32   glVertex1 = MatVecMul4x4(projModel, CREATE_VEC4f32(v1.x, v1.y, v1.z, 1.0f));
  struct Vec4f32   glVertex2 = MatVecMul4x4(projModel, CREATE_VEC4f32(v2.x, v2.y, v2.z, 1.0f));

  i32              xMin      = MIN(MIN(v0Proj.x, v1Proj.x), v2Proj.x);
  i32              yMin      = MIN(MIN(v0Proj.y, v1Proj.y), v2Proj.y);

  i32              xMax      = MAX(MAX(v0Proj.x, v1Proj.x), v2Proj.x);
  i32              yMax      = MAX(MAX(v0Proj.y, v1Proj.y), v2Proj.y);

  f32              totalArea = crossProduct2Df32(CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(v0Proj.x, v0Proj.y));

  for (i32 x = xMin; x <= xMax; x++)
  {

    for (i32 y = yMin; y <= yMax; y++)
    {
      f32  w0     = crossProduct2Df32(CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(x, y));
      f32  w1     = crossProduct2Df32(CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(v0Proj.x, v0Proj.y), CREATE_VEC2f32(x, y));
      f32  w2     = crossProduct2Df32(CREATE_VEC2f32(v0Proj.x, v0Proj.y), CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(x, y));

      f32  alpha  = w0 / totalArea;
      f32  beta   = w1 / totalArea;
      f32  gamma  = w2 / totalArea;

      bool inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

      i32  z      = alpha * v0Proj.z + beta * v1Proj.z + gamma * v2Proj.z;

      if (inside && zBuffer[y * image->width + x] < z)
      {
        zBuffer[y * image->width + x] = z;
        f32           u               = (alpha * uv0.x + beta * uv1.x + gamma * uv2.x);
        f32           v               = (alpha * uv0.y + beta * uv1.y + gamma * uv2.y);

        i32           tu              = u * texture->width;
        i32           tv              = v * texture->height;

        i32           uv              = (i32)(tu + tv * texture->width) * 4;

        struct Vec4u8 color           = {texture->data[uv + 2], texture->data[uv + 1], texture->data[uv + 0], 255};
        setPixel(image, x, y, color);
      }
    }
  }
}
