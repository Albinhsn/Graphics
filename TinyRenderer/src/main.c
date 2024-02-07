#include "targa.h"

int main() {
  struct TargaImage image;
  loadTarga(&image, "blizzard01.tga");
  saveTarga(&image, "output.tga");

  return 0;
}
