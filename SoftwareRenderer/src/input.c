#include "input.h"
#include "common.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

void getKeyboardInputCharacters(struct InputState* inputState, struct String* string)
{
  for (int i = 0; i < INPUT_STATE_LENGTH; i++)
  {
    if (inputState->keyboardStateRelease[i])
    {
      if (i == ASCII_RETURN)
      {
        continue;
      }
      if (i == ASCII_BACKSPACE)
      {
        if (string->len > 0)
        {
          string->buffer[string->len--] = 0;
        }
      }
      else
      {
        if (string->len == TEXT_MAX_LENGTH - 1)
        {
          return;
        }
        if (string->len == string->capacity)
        {
          string->capacity *= 2;
          string->buffer = realloc(string->buffer, string->capacity);
        }
        string->buffer[string->len] = i;
        string->len++;
      }
    }
  }
}
static inline void handleKeyRelease(bool* key, bool* previousDown, bool down)
{
  if (*previousDown == true && !down)
  {
    *key          = true;
    *previousDown = false;
  }
  else
  {
    *previousDown = down;
  }
}

static inline void handleMouseInput(struct InputState* inputState, bool down, ui8 button)
{
  switch (button)
  {
  case SDL_BUTTON_LEFT:
  {
    handleKeyRelease(&inputState->mr_1, &inputState->md_1, down);
    break;
  }
  case SDL_BUTTON_RIGHT:
  {
    handleKeyRelease(&inputState->mr_3, &inputState->md_3, down);
    break;
  }
  default:
  {
    break;
  }
  }
}

static inline void handleKeyboardInput(struct InputState* inputState, bool down, SDL_KeyCode sym)
{
  if (sym >= INPUT_STATE_LENGTH)
  {
    return;
  }
  if (inputState->keyboardStateDown[sym] == true && !down)
  {
    inputState->keyboardStateRelease[sym] = true;
  }
  inputState->keyboardStateDown[sym] = down;
}

static inline bool pressedEscape(SDL_Event event)
{
  return event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE;
}

bool handleInput(struct InputState* inputState)
{
  SDL_Event event;
  bool      mouseEvent = false;
  memset(&inputState->keyboardStateRelease[0], 0, INPUT_STATE_LENGTH);
  inputState->mr_1 = false;
  inputState->mr_3 = false;

  while (SDL_PollEvent(&event))
  {
    if (pressedEscape(event))
    {
      return true;
    }
    switch (event.type)
    {
    case SDL_QUIT:
    {
      return true;
    }
    case SDL_MOUSEMOTION:
    {
      mouseEvent            = true;
      inputState->mouseX    = event.motion.x;
      inputState->mouseY    = event.motion.y;
      inputState->mouseXRel = event.motion.xrel;
      inputState->mouseYRel = event.motion.yrel;
      break;
    }
    case SDL_MOUSEBUTTONUP:
    {
      handleMouseInput(inputState, false, event.button.button);
      break;
    }
    case SDL_MOUSEBUTTONDOWN:
    {
      handleMouseInput(inputState, true, event.button.button);
      break;
    }
    case SDL_KEYUP:
    {
      handleKeyboardInput(inputState, false, event.key.keysym.sym);
      break;
    }
    case SDL_KEYDOWN:
    {
      handleKeyboardInput(inputState, true, event.key.keysym.sym);
      break;
    }
    default:
    {
      break;
    }
    }
  }
#if DEBUG_INPUT
  for (int i = 0; i < INPUT_STATE_LENGTH; i++)
  {
    printf("%d ", inputState->keyboardState[i]);
  }
  printf("\n");

#endif
  if (!mouseEvent)
  {
    SDL_GetMouseState(&inputState->mouseX, &inputState->mouseY);
  }

  return false;
}
