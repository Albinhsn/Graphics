#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"

#define CREATE_VEC2i32(x, y)       ((struct Vec2i32){x, y})
#define CREATE_VEC3i32(x, y, z)    ((struct Vec3i32){x, y, z})
#define CREATE_VEC4i32(x, y, z, w) ((struct Vec4i32){x, y, z, w})

#define CREATE_VEC2f32(x, y)       ((struct Vec2f32){x, y})
#define CREATE_VEC3f32(x, y, z)    ((struct Vec3f32){x, y, z})
#define CREATE_VEC4f32(x, y, z, w) ((struct Vec4f32){x, y, z, w})

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
typedef struct Vec4f32 Vec4f32;

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
typedef struct Vec3f32 Vec3f32;

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
typedef struct Vec2f32 Vec2f32;

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
typedef struct Vec2i32 Vec2i32;

struct Vec3i32
{
  i32 x, y, z;
};
typedef struct Vec3i32 Vec3i32;

struct Vec4i32
{
  i32 x, y, z, w;
};
typedef struct Vec4i32 Vec4i32;

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
typedef struct Vec2i8 Vec2i8;

struct Vec2u8
{
  union
  {
    u8 pos[2];
    struct
    {
      u8 x;
      u8 y;
    };
  };
};
typedef struct Vec2u8 Vec2u8;

#endif
