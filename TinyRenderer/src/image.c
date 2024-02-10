#include "image.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static inline void setPixel(struct Image* image, ui16 x, ui16 y, struct Vec4ui8 color)
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

void setLine(struct Image* image, ui16 x0, ui16 y0, ui16 x1, ui16 y1, struct Vec4ui8 color)
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

void initImage(struct Image* image, ui16 width, ui16 height, ui8* data)
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

void fillTriangle(struct Image* image, struct Image* texture, struct Image* normalMap, struct Vec3i32 v0, struct Vec3i32 v1, struct Vec3i32 v2, struct Vec2f32 uv0, struct Vec2f32 uv1,
                  struct Vec2f32 uv2, struct Vec3f32 n0, struct Vec3f32 n1, struct Vec3f32 n2, i32* zBuffer, struct Matrix4x4 m, struct Vec3f32 v0Proj, struct Vec3f32 v1Proj, struct Vec3f32 v2Proj)
{

  i32 xMin      = MIN(MIN(v0.x, v1.x), v2.x);
  i32 yMin      = MIN(MIN(v0.y, v1.y), v2.y);

  i32 xMax      = MAX(MAX(v0.x, v1.x), v2.x);
  i32 yMax      = MAX(MAX(v0.y, v1.y), v2.y);

  i32 totalArea = crossProduct2D(CREATE_VEC2i32(v0.x, v0.y), CREATE_VEC2i32(v1.x, v1.y), CREATE_VEC2i32(v2.x, v2.y));

  for (i32 x = xMin; x < xMax; x++)
  {
    for (i32 y = yMin; y < yMax; y++)
    {
      struct Vec2i32 point  = {x, y};

      i32            w0     = crossProduct2D(CREATE_VEC2i32(v1.x, v1.y), CREATE_VEC2i32(v2.x, v2.y), point);
      i32            w1     = crossProduct2D(CREATE_VEC2i32(v2.x, v2.y), CREATE_VEC2i32(v0.x, v0.y), point);
      i32            w2     = crossProduct2D(CREATE_VEC2i32(v0.x, v0.y), CREATE_VEC2i32(v1.x, v1.y), point);

      f32            alpha  = w0 / (f32)totalArea;
      f32            beta   = w1 / (f32)totalArea;
      f32            gamma  = w2 / (f32)totalArea;

      bool           inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

      i32            z      = alpha * v0.z + beta * v1.z + gamma * v2.z;

      if (inside && zBuffer[y * image->width + x] < z)
      {
        zBuffer[y * image->width + x] = z;
        f32            u              = (alpha * uv0.x + beta * uv1.x + gamma * uv2.x);
        f32            v              = (alpha * uv0.y + beta * uv1.y + gamma * uv2.y);

        i32            tu             = u * texture->width;
        i32            tv             = v * texture->height;
        i32            uv             = (i32)(tu + tv * texture->width) * 4;

        struct Matrix3x3 varyingNM = {
          .i = {},
          .j = {},
          .k = {},
        }

        struct Vec3f32 bn             = {
            alpha * n0.x + beta * n1.x + gamma * n2.x, //
            alpha * n0.y + beta * n1.y + gamma * n2.y, //
            alpha * n0.z + beta * n1.z + gamma * n2.z  //
        };
        normalizeVec3(&bn);
        printf("%d %d\n", x,y);
        printf("bn: %lf %lf %lf\n", bn.x, bn.y, bn.z);

        struct Matrix3x3 A = {
            .i = {v1Proj.x - v0Proj.x, v1Proj.y - v0Proj.y, v1Proj.z - v0Proj.z}, //
            .j = {v2Proj.x - v0Proj.x, v2Proj.y - v0Proj.y, v2Proj.z - v0Proj.z}, //
            .k = {       bn.x,        bn.x,        bn.z}  //
        };
        printf("%lf %lf %lf\n", A.m[0][0], A.m[0][1], A.m[0][2]);
        printf("%lf %lf %lf\n", A.m[1][0], A.m[1][1], A.m[1][2]);
        printf("%lf %lf %lf\n", A.m[2][0], A.m[2][1], A.m[2][2]);
        printf("---\n");

        struct Matrix3x3 AI = invertMat3x3(A);
        printf("%lf %lf %lf\n", AI.m[0][0], AI.m[0][1], AI.m[0][2]);
        printf("%lf %lf %lf\n", AI.m[1][0], AI.m[1][1], AI.m[1][2]);
        printf("%lf %lf %lf\n", AI.m[2][0], AI.m[2][1], AI.m[2][2]);
        printf("---\n");
        exit(1);

        struct Vec3f32 i = MatVecMul3x3(AI, CREATE_VEC3f32(uv1.x - uv0.x, uv2.x - uv0.x, 0.0f));
        struct Vec3f32 j = MatVecMul3x3(AI, CREATE_VEC3f32(uv1.y - uv0.y, uv2.y - uv0.y, 0.0f));

        normalizeVec3(&i);
        normalizeVec3(&j);

        struct Matrix3x3 B = {
            .i = { i.x,  i.y,  i.z}, //
            .j = { j.x,  j.y,  j.z}, //
            .k = {bn.x, bn.y, bn.z}  //
        };

        struct Vec3f32 uvVec3 = {u, v, 0.0f};
        normalizeVec3(&uvVec3);
        struct Vec3f32 n = MatVecMul3x3(B, uvVec3);
        normalizeVec3(&n);

        f32 intensity = dotProductVec3(n, LIGHT_DIR);

        if (intensity > 0)
        {
          struct Vec4ui8 color = {texture->data[uv + 2] * intensity, texture->data[uv + 1] * intensity, texture->data[uv + 0] * intensity, 255};
          setPixel(image, x, y, color);
        }
      }
    }
  }
}
