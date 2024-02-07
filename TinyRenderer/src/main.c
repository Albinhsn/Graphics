#include "targa.h"

#define WIDTH 100
#define HEIGHT 101
#define PIXELS WIDTH *HEIGHT * 4

int main() {
  struct TargaImage image;
  ui8 data[PIXELS];

  initTargaImage(&image, WIDTH, HEIGHT, &data[0]);

  setPixel(&image, 0, 0, RED);
  debugTargaData(&image);

  saveTarga(&image, "output.tga");

  return 0;
}
