#include "common.h"
#include "input.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <string.h>

#include <time.h>

void initWindow(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture)
{
  SDL_Init(SDL_INIT_VIDEO);
  *window   = SDL_CreateWindow("client", 0, 0, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_OPENGL);
  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_PRESENTVSYNC);
  *texture  = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, SCREENWIDTH, SCREENHEIGHT);
}

static inline void renderFramebuffer(SDL_Renderer* renderer, SDL_Texture* texture, ui32* framebuffer)
{
  SDL_UpdateTexture(texture, NULL, framebuffer, SCREENWIDTH * sizeof(ui32));
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

i32 main()
{
  SDL_Window*   window;
  SDL_Renderer* renderer;
  SDL_Texture*  texture;
  initWindow(&window, &renderer, &texture);

  ui32              framebuffer[SCREENWIDTH * SCREENHEIGHT];

  struct InputState inputState;

  ui32              previousTick = 0;

  srand(time(NULL));
  for (int i = 0; i < SCREENHEIGHT * SCREENWIDTH; i++)
  {
    framebuffer[i] = 0;
  }

  while (true)
  {
    if (handleInput(&inputState))
    {
      break;
    }
    ui32 ticks = SDL_GetTicks();
    if (previousTick + 100 <= ticks)
    {
      for (i32 i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++)
      {
        ui8 r = rand() % 255;
        ui8 g = rand() % 255;
        ui8 b = rand() % 255;
        ui8 a = 0xFF;
        framebuffer[i] |= a << 24;
        framebuffer[i] |= b << 16;
        framebuffer[i] |= g << 8;
        framebuffer[i] |= r;
      }
      previousTick = ticks;
      renderFramebuffer(renderer, texture, framebuffer);
      for (i32 i = 0; i < SCREENWIDTH * SCREENHEIGHT; i++)
      {
        framebuffer[i] = 0;
      }
    }
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
