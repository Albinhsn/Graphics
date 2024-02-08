#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef unsigned long ui64;

typedef float f32;
typedef double f64;

typedef int8_t i8;
typedef int16_t i16;
typedef int i32;
typedef int64_t i64;

typedef struct Vec4ui8;

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x < y ? y : x)

#define RED ((struct Vec4ui8){255, 0, 0, 255})
#define YELLOW ((struct Vec4ui8){255, 255, 0, 255})
#define GREEN ((struct Vec4ui8){0, 255, 0, 255})
#define CYAN ((struct Vec4ui8){0, 255, 255, 255})
#define PURPLE ((struct Vec4ui8){255, 0, 255, 255})
#define BLUE ((struct Vec4ui8){255, 0, 0, 255})
#define WHITE ((struct Vec4ui8){255, 255, 255, 255})
#define BLACK ((struct Vec4ui8){0, 0, 0, 255})
#define PINK ((struct Vec4ui8){255, 192, 203, 255})

#endif
