#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"
#include "vector.h"
struct Image {
  ui16 width;
  ui16 height;
  ui8 *data;
};

void setLine(struct Image *image, ui16 x0, ui16 y0, ui16 x1, ui16 y1,
             struct Vec4ui8 color);
void initImage(struct Image *image, ui16 width, ui16 height, ui8 *data);
void debugImageData(struct Image *image);
void fillTriangle(struct Image *image, struct Image *texture, struct Vec3f32 v0,
                  struct Vec3f32 v1, struct Vec3f32 v2, struct Vec2f32 uv0,
                  struct Vec2f32 uv1, struct Vec2f32 uv2, f32 *zBuffer);

#endif
