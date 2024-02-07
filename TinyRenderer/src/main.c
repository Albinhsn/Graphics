#include "targa.h"

#define WIDTH 100
#define HEIGHT 100
#define PIXELS WIDTH *HEIGHT * 4

int main() {
  struct TargaImage image;
  ui8 data[PIXELS];

  initTargaImage(&image, WIDTH, HEIGHT, &data[0]);

  for (i32 i = 0; i < 1000000; i++) {
    setLine(&image, 13, 20, 80, 40, WHITE);
    setLine(&image, 20, 13, 40, 80, RED);
    setLine(&image, 80, 40, 13, 20, RED);
  }
  // saveTarga(&image, "output.tga");
  return 0;
}
