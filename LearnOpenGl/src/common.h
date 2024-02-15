#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <sys/time.h>

#define DEBUG_INPUT                       false
#define DEBUG_PLAYER                      false
#define DEBUG_SDL                         false
#define DEBUG_BOUNDS                      true

#define MAPFILE_LOCATION                  "./data/maps/dungeon_map01.txt"
#define TILEIMAGE_LOCATION                "./data/tiles/tile_data03.txt"

#define SCREENWIDTH                       1080
#define SCREENHEIGHT                      720

#define PLAYER_BOUND_WIDTH                SCREENWIDTH * 0.04f
#define PLAYER_BOUND_HEIGHT               SCREENHEIGHT * 0.12f
#define PLAYER_BOUND_OFFSET_X             0.0f
#define PLAYER_BOUND_OFFSET_Y             SCREENHEIGHT * -0.08f

#define MIN_COLLISION_AMOUNT              5.0f

#define VSYNC                             false

#define ONE_DIVIDED_BY_SCREENWIDTH        1.0f / (float)SCREENWIDTH
#define ONE_DIVIDED_BY_SCREENHEIGHT       1.0f / (float)SCREENHEIGHT

#define INBOUNDS(fst, snd)                (MAX(MIN(fst, snd), -snd))

#define MIN(fst, snd)                     (fst < snd ? fst : snd)
#define MAX(fst, snd)                     (fst > snd ? fst : snd)

#define WORLDSPACEX_TO_VIEWSPACE(x)       (((float)(x) * ONE_DIVIDED_BY_SCREENWIDTH))
#define WORLDSPACEY_TO_VIEWSPACE(y)       (((float)(y) * ONE_DIVIDED_BY_SCREENHEIGHT))

#define VIEWSPACEY_TO_WORLDSPACE(y)       ((y) * SCREENHEIGHT)
#define VIEWSPACEX_TO_WORLDSPACE(x)       ((x) * SCREENWIDTH)

#define MOUSEX_TO_WORLDSPACE(x)           ((float)(2.0f * x - SCREENWIDTH))
#define MOUSEY_TO_WORLDSPACE(x)           ((-(float)(2.0f * x - SCREENHEIGHT)))

#define VIEWSPACE_WIDTH_TO_WORLDSPACE(x)  ((struct Vec2f32){-SCREENWIDTH * x, SCREENWIDTH * x})
#define VIEWSPACE_HEIGHT_TO_WORLDSPACE(y) ((struct Vec2f32){-SCREENHEIGHT * y, SCREENHEIGHT * y})

#define ANIMATION_UPDATE_TIMER            100

#define BACKGROUND_FILE_LOCATION          "./Assets/Backgrounds/tile_0009.png"
#define FONT_IMAGE_LOCATION               "./Assets/Fonts/font01.png"
#define FONT_DATA_LOCATION                "./Assets/Fonts/font01.txt"

#define ABS_F(x)                          ((x < 0.0f ? -(x) : (x)))

#define ASCII_ESCAPE                      27
#define ASCII_RETURN                      13
#define ASCII_SPACE                       32
#define ASCII_BACKSPACE                   8

#define BACKGROUND_PROGRAM_VALUE          9

#define TEXT_PROGRAM_VALUE                3
#define TEXT_TEXTURE_VALUE                1

#define MOVEMENT_UPDATE_TIMER             16

typedef struct Vec4f32;

#define RED    ((struct Vec4f32){1.0f, 0.0f, 0.0f, 1.0f})
#define YELLOW ((struct Vec4f32){1.0f, 1.0f, 0.0f, 1.0f})
#define GREEN  ((struct Vec4f32){0.0f, 1.0f, 0.0f, 1.0f})
#define CYAN   ((struct Vec4f32){0.0f, 1.0f, 1.0f, 1.0f})
#define PURPLE ((struct Vec4f32){1.0f, 0.0f, 1.0f, 1.0f})
#define BLUE   ((struct Vec4f32){1.0f, 0.0f, 0.0f, 1.0f})

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef float    f32;
typedef double   f64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int      i32;
typedef int64_t  i64;

u32             timeInMilliseconds(void);
void             getInfoStrings(char* msString, char* fpsString, u32* lastTick, u32* previousTick);

#endif
