#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

typedef uint8_t       u8;
typedef uint16_t      u16;
typedef uint32_t      u32;
typedef unsigned long u64;

typedef float         f32;
typedef double        f64;

typedef int8_t        i8;
typedef int16_t       i16;
typedef int           i32;
typedef int64_t       i64;

typedef struct Vec4u8;
typedef struct Vec3f32;

#define MIN(x, y)                   (x < y ? x : y)
#define MAX(x, y)                   (x < y ? y : x)
#define PI                          3.14159265358979323846 /* pi */

#define LIGHT_DIR                   ((struct Vec3f32){-0.5f, 2.0f, 4.0f})

#define WIDTH                       800
#define HEIGHT                      800

#define DEPTH                       500

#define UP                          ((struct Vec3f32){0.0f, 1.0f, 0.0f})

#define VIEWSPACE_TO_WORLDSPACEY(x) (((x) + 1.0f) * (HEIGHT / 2.0f))
#define VIEWSPACE_TO_WORLDSPACEX(x) (((x) + 1.0f) * (WIDTH / 2.0f))

#define CAST_VEC4f32_TO_VEC3i32(v)  ((struct Vec3i32){v.x, v.y, v.z})
#define CAST_VEC4f32_TO_VEC3f32(v)  ((struct Vec3f32){v.x, v.y, v.z})
#define CAST_VEC3f32_TO_VEC2f32(v)  ((struct Vec2f32){v.x, v.y})

#define RED                         ((struct Vec4u8){255, 0, 0, 255})
#define YELLOW                      ((struct Vec4u8){255, 255, 0, 255})
#define GREEN                       ((struct Vec4u8){0, 255, 0, 255})
#define CYAN                        ((struct Vec4u8){0, 255, 255, 255})
#define PURPLE                      ((struct Vec4u8){255, 0, 255, 255})
#define BLUE                        ((struct Vec4u8){255, 0, 0, 255})
#define WHITE                       ((struct Vec4u8){255, 255, 255, 255})
#define BLACK                       ((struct Vec4u8){0, 0, 0, 255})
#define PINK                        ((struct Vec4u8){255, 192, 203, 255})

#endif
