#include "vector.h"
#include "common.h"
#include <math.h>

i32 crossProduct2D(struct Vec2i32 v0, struct Vec2i32 v1, struct Vec2i32 point) {
  struct Vec2i32 v0v1 = {v1.x - v0.x, v1.y - v0.y};
  struct Vec2i32 v0p = {point.x - v0.x, point.y - v0.y};

  return v0v1.x * v0p.y - v0v1.y * v0p.x;
}

f32 crossProduct2Df32(struct Vec2f32 v0, struct Vec2f32 v1,
                      struct Vec2f32 point) {
  struct Vec2f32 v0v1 = {v1.x - v0.x, v1.y - v0.y};
  struct Vec2f32 v0p = {point.x - v0.x, point.y - v0.y};

  return v0v1.x * v0p.y - v0v1.y * v0p.x;
}
struct Vec3f32 barycentric3D(struct Vec2f32 v0, struct Vec2f32 v1,
                             struct Vec2f32 v2, struct Vec2f32 point) {
  f32 total = crossProduct2Df32(v0, v1, v2);
  struct Vec3f32 b = {
      crossProduct2Df32(v0, v2, point) / total, //
      crossProduct2Df32(v1, v0, point) / total, //
      crossProduct2Df32(v2, v0, point) / total  //
  };

  return b;
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
