#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;

typedef float f32;
typedef double f64;

typedef int8_t i8;
typedef int16_t i16;
typedef int i32;
typedef int64_t i64;

typedef struct Vec4f32;

#define RED ((struct Vec4f32){1.0f, 0.0f, 0.0f, 1.0f})
#define YELLOW ((struct Vec4f32){1.0f, 1.0f, 0.0f, 1.0f})
#define GREEN ((struct Vec4f32){0.0f, 1.0f, 0.0f, 1.0f})
#define CYAN ((struct Vec4f32){0.0f, 1.0f, 1.0f, 1.0f})
#define PURPLE ((struct Vec4f32){1.0f, 0.0f, 1.0f, 1.0f})
#define BLUE ((struct Vec4f32){1.0f, 0.0f, 0.0f, 1.0f})

#endif
