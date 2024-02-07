#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <sys/time.h>

#define SCREENWIDTH        1080
#define SCREENHEIGHT       720

#define VSYNC              false

#define MIN(fst, snd)      (fst < snd ? fst : snd)
#define MAX(fst, snd)      (fst > snd ? fst : snd)

#define ABS_F(x)           ((x < 0.0f ? -(x) : (x)))

#define ASCII_ESCAPE       27
#define ASCII_RETURN       13
#define ASCII_SPACE        32
#define ASCII_BACKSPACE    8

#define TEXT_PROGRAM_VALUE 3
#define TEXT_TEXTURE_VALUE 1

#define ONE_DIVIDED_BY_SCREENWIDTH        1.0f / (float)SCREENWIDTH
#define ONE_DIVIDED_BY_SCREENHEIGHT       1.0f / (float)SCREENHEIGHT

#define WORLDSPACEX_TO_VIEWSPACE(x)       (((float)(x) * ONE_DIVIDED_BY_SCREENWIDTH))
#define WORLDSPACEY_TO_VIEWSPACE(y)       (((float)(y) * ONE_DIVIDED_BY_SCREENHEIGHT))

#define FONT false 

#if FONT
#define FONT_IMAGE_LOCATION ""
#define FONT_DATA_LOCATION ""
#endif

#define TEXT_MAX_LENGTH 32

typedef struct Vec4f32;

#define RED    ((struct Vec4f32){1.0f, 0.0f, 0.0f, 1.0f})
#define YELLOW ((struct Vec4f32){1.0f, 1.0f, 0.0f, 1.0f})
#define GREEN  ((struct Vec4f32){0.0f, 1.0f, 0.0f, 1.0f})
#define CYAN   ((struct Vec4f32){0.0f, 1.0f, 1.0f, 1.0f})
#define PURPLE ((struct Vec4f32){1.0f, 0.0f, 1.0f, 1.0f})
#define BLUE   ((struct Vec4f32){1.0f, 0.0f, 0.0f, 1.0f})

typedef uint8_t  ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;

typedef float    f32;
typedef double   f64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int      i32;
typedef int64_t  i64;

ui32             timeInMilliseconds(void);
void             getInfoStrings(char* msString, char* fpsString, ui32* lastTick, ui32* previousTick);

#endif
