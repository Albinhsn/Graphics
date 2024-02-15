#include "common.h"
#include "input.h"
#include "sdl.h"

int main()
{
  SDL_Window*   window;
  SDL_GLContext context;

  initSDLWindow(&context, SCREENWIDTH, SCREENHEIGHT);

  InputState inputState;
  initInputState(&inputState);

  while (true)
  {

    if (handleInput(&inputState))
    {
      break;
    }
    initNewFrame();

    SDL_GL_SwapWindow(window);
  }

  return 0;
}
