#include "image.h"
#include "common.h"
#include "vector.h"
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

void fillShadowBuffer(struct Image* shadowBuffer, struct Vec3f32 v0, struct Vec3f32 v1, struct Vec3f32 v2, struct Matrix4x4 viewport, struct Matrix4x4 projectionMatrix, struct Matrix4x4 modelView,
                      struct Vec3f32 center, i32* zBuffer)
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

  for (i32 x = xMin; x <= xMax; x++)
  {

    for (i32 y = yMin; y <= yMax; y++)
    {
      struct Vec2i32 point     = {x, y};

      f32            w0        = crossProduct2Df32(CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(point.x, point.y));
      f32            w1        = crossProduct2Df32(CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(v0Proj.x, v0Proj.y), CREATE_VEC2f32(point.x, point.y));
      f32            w2        = crossProduct2Df32(CREATE_VEC2f32(v0Proj.x, v0Proj.y), CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(point.x, point.y));

      f32            alpha     = w0 / glVertex0.w;
      f32            beta      = w1 / glVertex1.w;
      f32            gamma     = w2 / glVertex2.w;

      f32            totalArea = alpha + beta + gamma;
      alpha                    = alpha / totalArea;
      beta                     = beta / totalArea;
      gamma                    = gamma / totalArea;

      bool inside              = w0 >= 0 && w1 >= 0 && w2 >= 0;

      i32  z                   = alpha * v0Proj.z + beta * v1Proj.z + gamma * v2Proj.z;

      if (inside && zBuffer[y * shadowBuffer->width + x] < z)
      {
        zBuffer[y * shadowBuffer->width + x] = z;

        struct Matrix3x3 m                   = {
                              .i = {v0Proj.x, v1Proj.x, v2Proj.x}, //
                              .j = {v0Proj.y, v1Proj.y, v2Proj.y},
                              .k = {v0Proj.z, v1Proj.z, v2Proj.z}
        };
        struct Vec3f32 p     = MatVecMul3x3(m, CREATE_VEC3f32(alpha, beta, gamma));
        struct Vec4u8  color = {255 * p.z / DEPTH, 255 * p.z / DEPTH, 255 * p.z / DEPTH, 255};
        setPixel(shadowBuffer, x, y, color);
      }
    }
  }
}

void fillTriangle(struct Image* image, struct Image* texture, struct Image* normalMap, struct Vec3f32 v0, struct Vec3f32 v1, struct Vec3f32 v2, struct Vec2f32 uv0, struct Vec2f32 uv1,
                  struct Vec2f32 uv2, struct Vec3f32 n0, struct Vec3f32 n1, struct Vec3f32 n2, i32* zBuffer, struct Matrix4x4 viewport, struct Matrix4x4 projectionMatrix, struct Matrix4x4 modelView,
                  u8* shadowBuffer, struct Matrix4x4 shadowMatrix)
{

  struct Matrix4x4 projModel    = MatMul4x4(projectionMatrix, modelView);
  struct Matrix4x4 M            = MatMul4x4(viewport, projModel);
  struct Matrix4x4 invProjModel = invertMat4x4(projModel);

  struct Vec3f32   v0Proj       = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v0.x, v0.y, v0.z, 1.0f)));
  struct Vec3f32   v1Proj       = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v1.x, v1.y, v1.z, 1.0f)));
  struct Vec3f32   v2Proj       = ProjectVec4ToVec3(MatVecMul4x4(M, CREATE_VEC4f32(v2.x, v2.y, v2.z, 1.0f)));

  struct Vec3f32   n0Proj       = CAST_VEC4f32_TO_VEC3f32(MatVecMul4x4(invProjModel, CREATE_VEC4f32(n0.x, n0.y, n0.z, 0.0f)));
  struct Vec3f32   n1Proj       = CAST_VEC4f32_TO_VEC3f32(MatVecMul4x4(invProjModel, CREATE_VEC4f32(n1.x, n1.y, n1.z, 0.0f)));
  struct Vec3f32   n2Proj       = CAST_VEC4f32_TO_VEC3f32(MatVecMul4x4(invProjModel, CREATE_VEC4f32(n2.x, n2.y, n2.z, 0.0f)));

  struct Vec4f32   glVertex0    = MatVecMul4x4(projModel, CREATE_VEC4f32(v0.x, v0.y, v0.z, 1.0f));
  struct Vec4f32   glVertex1    = MatVecMul4x4(projModel, CREATE_VEC4f32(v1.x, v1.y, v1.z, 1.0f));
  struct Vec4f32   glVertex2    = MatVecMul4x4(projModel, CREATE_VEC4f32(v2.x, v2.y, v2.z, 1.0f));

  struct Vec3f32   ndc_tri0     = ProjectVec4ToVec3(glVertex0);
  struct Vec3f32   ndc_tri1     = ProjectVec4ToVec3(glVertex1);
  struct Vec3f32   ndc_tri2     = ProjectVec4ToVec3(glVertex2);

  i32              xMin         = MIN(MIN(v0Proj.x, v1Proj.x), v2Proj.x);
  i32              yMin         = MIN(MIN(v0Proj.y, v1Proj.y), v2Proj.y);

  i32              xMax         = MAX(MAX(v0Proj.x, v1Proj.x), v2Proj.x);
  i32              yMax         = MAX(MAX(v0Proj.y, v1Proj.y), v2Proj.y);

  f32              totalArea    = crossProduct2Df32(CREATE_VEC2f32(v1Proj.x, v1Proj.y), CREATE_VEC2f32(v2Proj.x, v2Proj.y), CREATE_VEC2f32(v0Proj.x, v0Proj.y));

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
        f32 u                         = (alpha * uv0.x + beta * uv1.x + gamma * uv2.x);
        f32 v                         = (alpha * uv0.y + beta * uv1.y + gamma * uv2.y);

        i32 tu                        = u * texture->width;
        i32 tv                        = v * texture->height;

        i32 uv                        = (i32)(tu + tv * texture->width) * 4;

        // bn
        struct Vec3f32 bn = {
            .x = alpha * n0Proj.x + beta * n1Proj.x + gamma * n2Proj.x, //
            .y = alpha * n0Proj.y + beta * n1Proj.y + gamma * n2Proj.y, //
            .z = alpha * n0Proj.z + beta * n1Proj.z + gamma * n2Proj.z, //
        };
        normalizeVec3(&bn);
        struct Matrix3x3 A = {
            .i = {ndc_tri1.x - ndc_tri0.x, ndc_tri1.y - ndc_tri0.y, ndc_tri1.z - ndc_tri0.z},
            .j = {ndc_tri2.x - ndc_tri0.x, ndc_tri2.y - ndc_tri0.y, ndc_tri2.z - ndc_tri0.z},
            .k = {                   bn.x,                    bn.y,                    bn.z},
        };

        struct Matrix3x3 AI = invertMat3x3(A);
        struct Vec3f32   i  = MatVecMul3x3(AI, CREATE_VEC3f32(uv1.x - uv0.x, uv2.x - uv0.x, 0.0f));
        struct Vec3f32   j  = MatVecMul3x3(AI, CREATE_VEC3f32(uv1.y - uv0.y, uv2.y - uv0.y, 0.0f));

        normalizeVec3(&i);
        normalizeVec3(&j);

        struct Matrix3x3 B = {
            .i = {i.x, j.x, bn.x}, //
            .j = {i.y, j.y, bn.y},
            .k = {i.z, j.z, bn.z}
        };

        struct Vec3f32 normal = {(f32)normalMap->data[uv + 2] / 255.0f * 2.0f - 1.0f, (f32)normalMap->data[uv + 1] / 255.0f * 2.0f - 1.0f, (f32)normalMap->data[uv + 0] / 255.0f * 2.0f - 1.0f};
        struct Vec3f32 n      = MatVecMul3x3(B, normal);
        normalizeVec3(&n);

        struct Vec3f32 projLight = CAST_VEC4f32_TO_VEC3f32(MatVecMul4x4(projModel, CREATE_VEC4f32(LIGHT_DIR.x, LIGHT_DIR.y, LIGHT_DIR.z, 0.0f)));
        normalizeVec3(&projLight);

        f32 intensity       = dotProductVec3(n, projLight);
        intensity           = intensity < 0 ? 0 : intensity;

        struct Matrix3x3 vM = {
            .i = {v0Proj.x, v1Proj.x, v2Proj.x},
            .j = {v0Proj.y, v1Proj.y, v2Proj.y},
            .k = {v0Proj.z, v1Proj.z, v2Proj.z},
        };
        struct Vec3f32 vBar              = MatVecMul3x3(vM, CREATE_VEC3f32(alpha, beta, gamma));
        struct Vec4f32 shadowCoordinates = MatVecMul4x4(shadowMatrix, CREATE_VEC4f32(vBar.x, vBar.y, vBar.z, 1.0f));
        struct Vec3f32 shadowCoords      = ProjectVec4ToVec3(shadowCoordinates);

        i32            shadowIdx         = ((i32)shadowCoords.x) + ((i32)shadowCoords.y) * texture->width;
        f32            shadow            = 0.15f + 0.7f * (shadowBuffer[shadowIdx] < shadowCoords.z);

        struct Vec4u8  color             = {texture->data[uv + 2], texture->data[uv + 1], texture->data[uv + 0], 255};
        color.x                          = color.x * shadow + color.x * intensity;
        color.y                          = color.y * shadow + color.y * intensity;
        color.z                          = color.z * shadow + color.z * intensity;
        setPixel(image, x, y, color);
      }
    }
  }
}
