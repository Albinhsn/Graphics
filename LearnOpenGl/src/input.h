#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include "sta_string.h"
#include <stdbool.h>

#define INPUT_STATE_LENGTH 127

struct InputState
{
  i32  mouseX;
  i32  mouseY;
  i32  mouseXRel;
  i32  mouseYRel;
  bool md_1;
  bool mr_1;
  bool md_3;
  bool mr_3;
  bool keyboardStateRelease[INPUT_STATE_LENGTH];
  bool keyboardStateDown[INPUT_STATE_LENGTH];
};
typedef struct InputState InputState;

bool        handleInput(InputState* inputState);
void initInputState(InputState* inputState);
void getKeyboardInputCharacters(InputState* inputState, struct String* string);

#endif
