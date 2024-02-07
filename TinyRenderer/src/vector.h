#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"

#define CREATE_VEC2i32(x, y) ((struct Vec2i32){x, y})
#define CREATE_VEC3i32(x, y, z) ((struct Vec3i32){x, y, z})
#define CREATE_VEC4i32(x, y, z, w) ((struct Vec4i32){x, y, z, w})

#define CREATE_VEC2f32(x, y) ((struct Vec2f32){x, y})
#define CREATE_VEC3f32(x, y, z) ((struct Vec3f32){x, y, z})
#define CREATE_VEC4f32(x, y, z, w) ((struct Vec4f32){x, y, z, w})

struct Vec4ui8 {
  union {
    ui8 pos[4];
    struct {
      ui8 x;
      ui8 y;
      ui8 z;
      ui8 w;
    };
    struct {
      ui8 r;
      ui8 g;
      ui8 b;
      ui8 a;
    };
  };
};

struct Vec4f32 {
  union {
    f32 pos[4];
    struct {
      f32 x;
      f32 y;
      f32 z;
      f32 w;
    };
    struct {
      f32 r;
      f32 g;
      f32 b;
      f32 a;
    };
  };
};

struct Vec3f32 {
  union {
    f32 pos[3];
    struct {
      f32 x;
      f32 y;
      f32 z;
    };
    struct {
      f32 r;
      f32 g;
      f32 b;
    };
  };
};

struct Vec2f32 {
  union {
    f32 pos[2];
    struct {
      f32 x;
      f32 y;
    };
  };
};

struct Vec2i32 {
  union {
    i32 pos[2];
    struct {
      i32 x;
      i32 y;
    };
  };
};

struct Vec3i32 {
  i32 x, y, z;
};

struct Vec4i32 {
  i32 x, y, z, w;
};
struct Vec2i8 {
  union {
    i8 pos[2];
    struct {
      i8 x;
      i8 y;
    };
  };
};

struct Vec2ui8 {
  union {
    ui8 pos[2];
    struct {
      ui8 x;
      ui8 y;
    };
  };
};

#endif
