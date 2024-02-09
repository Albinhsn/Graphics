#include "vector.h"
#include "common.h"
#include <math.h>

struct Vec3f32 VecMul3f32(struct Vec3f32 a, struct Vec3f32 b)
{
  struct Vec3f32 res = {
      a.x * b.x, //
      a.y * b.y, //
      a.z * b.z  //
  };

  return res;
}

struct Vec4f32 MatVecMul4x4(struct Matrix4x4 mat, struct Vec4f32 vec)
{
  struct Vec4f32 res = {
      mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3] * vec.w, //
      mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3] * vec.w, //
      mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3] * vec.w, //
      mat.m[3][0] * vec.x + mat.m[3][1] * vec.y + mat.m[3][2] * vec.z + mat.m[3][3] * vec.w, //
                                                                                             //
  };
  return res;
}

struct Matrix4x4 lookAt(struct Vec3f32 eye, struct Vec3f32 center, struct Vec3f32 up)
{

  struct Vec3f32 z = {eye.x - center.x, eye.y - center.y, eye.z - center.z};
  normalizeVec3(&z);

  struct Vec3f32 x = crossProduct3D(up, z);
  normalizeVec3(&x);

  struct Vec3f32 y = crossProduct3D(z, x);
  normalizeVec3(&y);

  struct Matrix4x4 minV;
  buildIdentityMatrix4x4(&minV);

  struct Matrix4x4 tr;
  buildIdentityMatrix4x4(&tr);

  for (i32 i = 0; i < 3; i++)
  {
    minV.m[0][i] = x.pos[i];
    minV.m[1][i] = y.pos[i];
    minV.m[2][i] = z.pos[i];
    tr.m[i][3]   = -eye.pos[i];
  }
  return MatMul4x4(minV, tr);
}
struct Matrix4x4 MatMul4x4(struct Matrix4x4 a, struct Matrix4x4 b)
{
  f32              a00 = a.m[0][0], a01 = a.m[0][1], a02 = a.m[0][2], a03 = a.m[0][3];
  f32              a10 = a.m[1][0], a11 = a.m[1][1], a12 = a.m[1][2], a13 = a.m[1][3];
  f32              a20 = a.m[2][0], a21 = a.m[2][1], a22 = a.m[2][2], a23 = a.m[2][3];
  f32              a30 = a.m[3][0], a31 = a.m[3][1], a32 = a.m[3][2], a33 = a.m[3][3];

  f32              b00 = b.m[0][0], b01 = b.m[0][1], b02 = b.m[0][2], b03 = b.m[0][3];
  f32              b10 = b.m[1][0], b11 = b.m[1][1], b12 = b.m[1][2], b13 = b.m[1][3];
  f32              b20 = b.m[2][0], b21 = b.m[2][1], b22 = b.m[2][2], b23 = b.m[2][3];
  f32              b30 = b.m[3][0], b31 = b.m[3][1], b32 = b.m[3][2], b33 = b.m[3][3];

  struct Matrix4x4 m;
  m.m[0][0] = a00 * b00 + a01 * b10 + a02 * b20 + a03 * b30;
  m.m[0][1] = a00 * b01 + a01 * b11 + a02 * b21 + a03 * b31;
  m.m[0][2] = a00 * b02 + a01 * b12 + a02 * b22 + a03 * b32;
  m.m[0][3] = a00 * b03 + a01 * b13 + a02 * b23 + a03 * b33;

  m.m[1][0] = a10 * b00 + a11 * b10 + a12 * b20 + a13 * b30;
  m.m[1][1] = a10 * b01 + a11 * b11 + a12 * b21 + a13 * b31;
  m.m[1][2] = a10 * b02 + a11 * b12 + a12 * b22 + a13 * b32;
  m.m[1][3] = a10 * b03 + a11 * b13 + a12 * b23 + a13 * b33;

  m.m[2][0] = a20 * b00 + a21 * b10 + a22 * b20 + a23 * b30;
  m.m[2][1] = a20 * b01 + a21 * b11 + a22 * b21 + a23 * b31;
  m.m[2][2] = a20 * b02 + a21 * b12 + a22 * b22 + a23 * b32;
  m.m[2][3] = a20 * b03 + a21 * b13 + a22 * b23 + a23 * b33;

  m.m[3][0] = a30 * b00 + a31 * b10 + a32 * b20 + a33 * b30;
  m.m[3][1] = a30 * b01 + a31 * b11 + a32 * b21 + a33 * b31;
  m.m[3][2] = a30 * b02 + a31 * b12 + a32 * b22 + a33 * b32;
  m.m[3][3] = a30 * b03 + a31 * b13 + a32 * b23 + a33 * b33;

  return m;
}

struct Matrix4x4 Vec3f32ToMatrix(struct Vec3f32 v)
{
  struct Matrix4x4 m;
  buildIdentityMatrix4x4(&m);
  m.m[0][0] = v.x;
  m.m[1][0] = v.y;
  m.m[2][0] = v.z;
  m.m[3][0] = 1.0f;

  return m;
}

struct Vec3i32 MatrixToVec3f32(struct Matrix4x4 m)
{
  struct Vec3i32 v = {m.m[0][0] / m.m[3][0], m.m[1][0] / m.m[3][0], m.m[2][0] / m.m[3][0]};

  return v;
}
void buildViewportMatrix4x4(struct Matrix4x4* m, i32 x, i32 y, i32 w, i32 h, i32 depth)
{
  m->m[0][3] = x + w / 2.0f;
  m->m[1][3] = y + h / 2.0f;
  m->m[2][3] = depth / 2.0f;

  m->m[0][0] = w / 2.0f;
  m->m[1][1] = h / 2.0f;
  m->m[2][2] = depth / 2.0f;
}
void buildIdentityMatrix4x4(struct Matrix4x4* m)
{
  m->m[0][0] = 1;
  m->m[0][1] = 0;
  m->m[0][2] = 0;
  m->m[0][3] = 0;

  m->m[1][0] = 0;
  m->m[1][1] = 1;
  m->m[1][2] = 0;
  m->m[1][3] = 0;

  m->m[2][0] = 0;
  m->m[2][1] = 0;
  m->m[2][2] = 1;
  m->m[2][3] = 0;

  m->m[3][0] = 0;
  m->m[3][1] = 0;
  m->m[3][2] = 0;
  m->m[3][3] = 1;
}

i32 crossProduct2D(struct Vec2i32 v0, struct Vec2i32 v1, struct Vec2i32 point)
{
  struct Vec2i32 v0v1 = {v1.x - v0.x, v1.y - v0.y};
  struct Vec2i32 v0p  = {point.x - v0.x, point.y - v0.y};

  return v0v1.x * v0p.y - v0v1.y * v0p.x;
}

f32 crossProduct2Df32(struct Vec2f32 v0, struct Vec2f32 v1, struct Vec2f32 point)
{
  struct Vec2f32 v0v1 = {v1.x - v0.x, v1.y - v0.y};
  struct Vec2f32 v0p  = {point.x - v0.x, point.y - v0.y};

  return v0v1.x * v0p.y - v0v1.y * v0p.x;
}
struct Vec3f32 barycentric3D(struct Vec2f32 v0, struct Vec2f32 v1, struct Vec2f32 v2, struct Vec2f32 point)
{
  f32            total = crossProduct2Df32(v0, v1, v2);
  struct Vec3f32 b     = {
      crossProduct2Df32(v0, v2, point) / total, //
      crossProduct2Df32(v1, v0, point) / total, //
      crossProduct2Df32(v2, v0, point) / total  //
  };

  return b;
}

struct Vec3f32 vectorSubtraction(struct Vec3f32 a, struct Vec3f32 b)
{
  struct Vec3f32 res = {
      a.x - b.x, //
      a.y - b.y, //
      a.z - b.z  //
  };

  return res;
}

void normalizeVec3(struct Vec3f32* v)
{
  f32 len = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
  v->x /= len;
  v->y /= len;
  v->z /= len;
}

f32 dotProductVec3(struct Vec3f32 a, struct Vec3f32 b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vec3f32 crossProduct3D(struct Vec3f32 a, struct Vec3f32 b)
{
  struct Vec3f32 res = {
      a.y * b.z - a.z * b.y, //
      a.z * b.x - a.x * b.z, //
      a.x * b.y - a.y * b.x  //
  };

  return res;
}
