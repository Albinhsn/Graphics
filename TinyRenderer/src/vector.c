#include "vector.h"
#include "common.h"
#include <math.h>
#include <stdio.h>

i32 crossProduct2D(struct Vec2i32 point, struct Vec2i32 v0, struct Vec2i32 v1) {
  struct Vec2i32 v0v1 = {v1.x - v0.x, v1.y - v0.y};
  struct Vec2i32 v0p = {point.x - v0.x, point.y - v0.y};

  return v0v1.x * v0p.y - v0v1.y * v0p.x;
}

struct Vec3f32 vectorSubtraction(struct Vec3f32 a, struct Vec3f32 b) {
  struct Vec3f32 res = {
      a.x - b.x, //
      a.y - b.y, //
      a.z - b.z  //
  };

  return res;
}

void normalizeVec3(struct Vec3f32 *v) {
  f32 len = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
  v->x /= len;
  v->y /= len;
  v->z /= len;
}

f32 dotProductVec3(struct Vec3f32 a, struct Vec3f32 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vec3f32 crossProduct3D(struct Vec3f32 a, struct Vec3f32 b) {
  struct Vec3f32 res = {
      a.y * b.z - a.z * b.y, //
      a.z * b.x - a.x * b.z, //
      a.x * b.y - a.y * b.x  //
  };

  return res;
}
