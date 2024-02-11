#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"
#include "vector.h"
struct Image {
  u32 width;
  u32 height;
  u8 *data;
};

void setLine(struct Image *image, u16 x0, u16 y0, u16 x1, u16 y1,
             struct Vec4u8 color);
void initImage(struct Image *image, u16 width, u16 height, u8 *data);
void debugImageData(struct Image *image);
void fillTriangle(struct Image* image, struct Image* texture, struct Image* normalMap, struct Vec3f32 v0, struct Vec3f32 v1, struct Vec3f32 v2, struct Vec2f32 uv0, struct Vec2f32 uv1,
                  struct Vec2f32 uv2, struct Vec3f32 n0, struct Vec3f32 n1, struct Vec3f32 n2, i32* zBuffer, struct Matrix4x4 viewport, struct Matrix4x4 projectionMatrix, struct Matrix4x4 modelView,
                  u8* shadowBuffer, struct Matrix4x4 shadowMatrix);
void fillShadowBuffer(struct Image* shadowBuffer, struct Vec3f32 v0, struct Vec3f32 v1, struct Vec3f32 v2, struct Matrix4x4 viewport, struct Matrix4x4 projectionMatrix, struct Matrix4x4 modelView, struct Vec3f32 center,
                      i32* zBuffer);

#endif
