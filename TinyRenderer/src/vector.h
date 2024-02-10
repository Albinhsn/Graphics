#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"

#define CREATE_VEC2i32(x, y)       ((struct Vec2i32){x, y})
#define CREATE_VEC3i32(x, y, z)    ((struct Vec3i32){x, y, z})
#define CREATE_VEC4i32(x, y, z, w) ((struct Vec4i32){x, y, z, w})

#define CREATE_VEC2f32(x, y)       ((struct Vec2f32){x, y})
#define CREATE_VEC3f32(x, y, z)    ((struct Vec3f32){x, y, z})
#define CREATE_VEC4f32(x, y, z, w) ((struct Vec4f32){x, y, z, w})

struct Vec4ui8
{
  union
  {
    ui8 pos[4];
    struct
    {
      ui8 x;
      ui8 y;
      ui8 z;
      ui8 w;
    };
    struct
    {
      ui8 r;
      ui8 g;
      ui8 b;
      ui8 a;
    };
  };
};

struct Vec4f32
{
  union
  {
    f32 pos[4];
    struct
    {
      f32 x;
      f32 y;
      f32 z;
      f32 w;
    };
    struct
    {
      f32 r;
      f32 g;
      f32 b;
      f32 a;
    };
  };
};

struct Vec3ui32
{
  union
  {
    ui32 pos[3];
    struct
    {
      ui32 x;
      ui32 y;
      ui32 z;
    };
    struct
    {
      ui32 r;
      ui32 g;
      ui32 b;
    };
  };
};

struct Vec3f32
{
  union
  {
    f32 pos[3];
    struct
    {
      f32 x;
      f32 y;
      f32 z;
    };
    struct
    {
      f32 r;
      f32 g;
      f32 b;
    };
  };
};

struct Vec2f32
{
  union
  {
    f32 pos[2];
    struct
    {
      f32 x;
      f32 y;
    };
  };
};

struct Vec2i32
{
  union
  {
    i32 pos[2];
    struct
    {
      i32 x;
      i32 y;
    };
  };
};

struct Vec3i32
{
  i32 x, y, z;
};

struct Vec4i32
{
  i32 x, y, z, w;
};
struct Vec2i8
{
  union
  {
    i8 pos[2];
    struct
    {
      i8 x;
      i8 y;
    };
  };
};
struct Vec3ui8
{
  union
  {
    ui8 pos[3];
    struct
    {
      ui8 x;
      ui8 y;
      ui8 z;
    };
  };
};

struct Vec2ui8
{
  union
  {
    ui8 pos[2];
    struct
    {
      ui8 x;
      ui8 y;
    };
  };
};

struct Matrix4x4
{
  f32 m[4][4];
};
struct Matrix3x3
{
  union
  {
    f32 m[3][3];
    struct
    {
      f32 i[3];
      f32 j[3];
      f32 k[3];
    };
  };
};

void             buildIdentityMatrix4x4(struct Matrix4x4* m);
void             buildViewportMatrix4x4(struct Matrix4x4* m, i32 x, i32 y, i32 w, i32 h, i32 depth);

struct Matrix4x4 lookAt(struct Vec3f32 eye, struct Vec3f32 center, struct Vec3f32 up);

struct Vec3f32   VecMul3f32(struct Vec3f32 a, struct Vec3f32 b);

struct Matrix3x3 invertMat3x3(struct Matrix3x3 m);
struct Matrix3x3 MatMul3x3(struct Matrix3x3 m1, struct Matrix3x3 m2);
struct Matrix4x4 MatMul4x4(struct Matrix4x4 m1, struct Matrix4x4 m2);
struct Vec4f32   MatVecMul4x4(struct Matrix4x4 mat, struct Vec4f32 vec);
struct Vec3f32 MatVecMul3x3(struct Matrix3x3 m, struct Vec3f32 v);

struct Vec3i32   MatrixToVec3f32(struct Matrix4x4 m);

struct Matrix4x4 Vec3f32ToMatrix(struct Vec3f32 v);
struct Vec3f32   vectorSubtraction(struct Vec3f32 a, struct Vec3f32 b);
void             normalizeVec3(struct Vec3f32* v);
f32              dotProductVec3(struct Vec3f32 a, struct Vec3f32 b);

struct Vec3f32   barycentric3D(struct Vec2f32 v0, struct Vec2f32 v1, struct Vec2f32 v2, struct Vec2f32 point);

i32              crossProduct2D(struct Vec2i32 v0, struct Vec2i32 v1, struct Vec2i32 point);
struct Vec3f32   crossProduct3D(struct Vec3f32 a, struct Vec3f32 b);
i32              crossProduct3DVector(struct Vec3i32 a, struct Vec3i32 b, struct Vec3i32 p);

struct Vec4f32   matMul(struct Vec4f32 a, struct Vec4f32 b);
#endif
