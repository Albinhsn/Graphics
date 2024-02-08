#include "image.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static inline void setPixel(struct Image *image, ui16 x, ui16 y,
                            struct Vec4ui8 color) {
  i32 idxOffset = (y * image->width + x) * 4;
  image->data[idxOffset + 0] = color.b;
  image->data[idxOffset + 1] = color.g;
  image->data[idxOffset + 2] = color.r;
  image->data[idxOffset + 3] = color.a;
}

static inline i16 absDiff(i16 x, i16 y) {
  i16 diff = x - y;
  return diff < 0 ? -diff : diff;
}

static inline i16 absi16(i16 x) { return x < 0 ? -x : x; }

void setLine(struct Image *image, ui16 x0, ui16 y0, ui16 x1, ui16 y1,
             struct Vec4ui8 color) {

  bool steep = absi16(x0 - x1) < absi16(y0 - y1);

  if (steep) {
    i16 tmp = x0;
    x0 = y0;
    y0 = tmp;

    tmp = x1;
    x1 = y1;
    y1 = tmp;
  }

  if (x0 > x1) {
    i16 tmp = x0;
    x0 = x1;
    x1 = tmp;

    tmp = y0;
    y0 = y1;
    y1 = tmp;
  }

  i16 dx = x1 - x0;
  i16 dy = y1 - y0;

  i16 d = 2 * dy;
  bool downwards = d < 0;
  d = downwards ? -d : d;

  i16 derror = 0;
  i16 y = y0;
  for (i16 x = x0; x <= x1; x++) {
    setPixel(image, steep ? y : x, steep ? x : y, color);
    derror += d;
    if (derror > dx) {
      y = downwards ? y - 1 : y + 1;
      derror -= 2 * dx;
    }
  }
}

void initImage(struct Image *image, ui16 width, ui16 height, ui8 *data) {

  image->width = width;
  image->height = height;
  for (i32 i = 0; i < width * height * 4; i += 4) {
    data[i + 0] = 0;
    data[i + 1] = 0;
    data[i + 2] = 0;
    data[i + 3] = 0xFF;
  }

  image->data = data;
}

void debugImageData(struct Image *image) {
  i32 pixelRowLength = image->width * 4;
  for (i32 y = 0; y < image->height; y++) {
    for (int x = 0; x < pixelRowLength; x += 4) {
      printf("|%d %d %d %d", image->data[y * pixelRowLength + x + 0],
             image->data[y * pixelRowLength + x + 1],
             image->data[y * pixelRowLength + x + 2],
             image->data[y * pixelRowLength + x + 3]);
    }
    printf("|\n");
  }
}

void fillTriangle(struct Image *image, struct Image *texture, struct Vec3f32 v0,
                  struct Vec3f32 v1, struct Vec3f32 v2, struct Vec2f32 uv0,
                  struct Vec2f32 uv1, struct Vec2f32 uv2, f32 *zBuffer) {
  struct Vec2i32 v0i32 = CREATE_VEC2i32(VIEWSPACE_TO_WORLDSPACEX(v0.x),
                                        VIEWSPACE_TO_WORLDSPACEY(v0.y));
  struct Vec2i32 v1i32 = CREATE_VEC2i32(VIEWSPACE_TO_WORLDSPACEX(v1.x),
                                        VIEWSPACE_TO_WORLDSPACEY(v1.y));
  struct Vec2i32 v2i32 = CREATE_VEC2i32(VIEWSPACE_TO_WORLDSPACEX(v2.x),
                                        VIEWSPACE_TO_WORLDSPACEY(v2.y));

  i32 xMin = MIN(MIN(v0i32.x, v1i32.x), v2i32.x);
  i32 yMin = MIN(MIN(v0i32.y, v1i32.y), v2i32.y);

  i32 xMax = MAX(MAX(v0i32.x, v1i32.x), v2i32.x);
  i32 yMax = MAX(MAX(v0i32.y, v1i32.y), v2i32.y);


  i32 totalArea = crossProduct2D(CREATE_VEC2i32(v0i32.x, v0i32.y),
                                 CREATE_VEC2i32(v1i32.x, v1i32.y),
                                 CREATE_VEC2i32(v2i32.x, v2i32.y));

  for (i32 x = xMin; x < xMax; x++) {
    for (i32 y = yMin; y < yMax; y++) {
      struct Vec2i32 point = {x, y};

      i32 w0 = crossProduct2D(v1i32, v2i32, point);
      i32 w1 = crossProduct2D(v2i32, v0i32, point);
      i32 w2 = crossProduct2D(v0i32, v1i32, point);

      f32 alpha = w0 / (f32)totalArea;
      f32 beta = w1 / (f32)totalArea;
      f32 gamma = w2 / (f32)totalArea;

      bool inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

      f32 z = alpha * v0.z + beta * v1.z + gamma * v2.z;

      if (inside && zBuffer[y * image->width + x] < z) {
        zBuffer[y * image->width + x] = z;
        f32 u = (alpha * uv0.x + beta * uv1.x + gamma * uv2.x);
        f32 v = (alpha * uv0.y + beta * uv1.y + gamma * uv2.y);

        i32 tu = u * texture->width;
        i32 tv = v * texture->height;
        i32 tIdx = (i32)(tu + tv * texture->width) * 4;

        struct Vec4ui8 color = {texture->data[tIdx + 2],
                                texture->data[tIdx + 1],
                                texture->data[tIdx + 0], 255};

        // printf("%d %d %d\n", color.x, color.y, color.z);
        // printf("%d\n", tIdx % 4);
        setPixel(image, x, y, color);
      }
    }
  }
}
