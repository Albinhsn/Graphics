#include "common.h"
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <string.h>

int quadProgram;

u32       timeInMilliseconds(void)
{
  return SDL_GetTicks();
}

void getInfoStrings(char* msString, char* fpsString, u32* lastTick, u32* previousTick)
{
  memset(msString, 0, strlen(msString));
  memset(fpsString, 0, strlen(fpsString));

  char tempString[16];
  memcpy(msString, "ms: ", 4);
  memcpy(fpsString, "fps: ", 5);

  int ms = *lastTick - *previousTick;
  if (ms > 9999)
  {
    ms = 9999;
  }

  *previousTick = *lastTick;
  sprintf(tempString, "%d", ms);
  strcat(msString, tempString);

  float fps = (1.0f / (float)ms) * 1000;
  sprintf(tempString, "%d", (int)fps);
  strcat(fpsString, tempString);
}
