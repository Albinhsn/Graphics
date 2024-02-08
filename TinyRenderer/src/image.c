#include "image.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>

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


void fillTriangle(struct Image *image, struct Vec2i32 v0, struct Vec2i32 v1,
                  struct Vec2i32 v2, struct Vec4ui8 color) {
  i32 xMin = MIN(MIN(v0.x, v1.x), v2.x);
  i32 yMin = MIN(MIN(v0.y, v1.y), v2.y);

  i32 xMax = MAX(MAX(v0.x, v1.x), v2.x);
  i32 yMax = MAX(MAX(v0.y, v1.y), v2.y);

  for (i32 x = xMin; x < xMax; x++) {
    for (i32 y = yMin; y < yMax; y++) {
      struct Vec2i32 point = {x, y};

      i32 w0 = crossProduct2D(v1, v2, point);
      i32 w1 = crossProduct2D(v2, v0, point);
      i32 w2 = crossProduct2D(v0, v1, point);

      bool inside = w0 >= 0 && w1 >= 0 && w2 >= 0;

      if (inside) {
        setPixel(image, x, y, color);
      }
    }
  }
}
