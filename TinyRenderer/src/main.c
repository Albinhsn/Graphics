#include "common.h"
#include "files.h"
#include "image.h"
#include <stdio.h>

#define WIDTH 1000
#define HEIGHT 1000

int main() {
  ui8 data[WIDTH * HEIGHT * 4];
  struct Image image;
  initImage(&image, WIDTH, HEIGHT, data);

  struct WavefrontObject obj;
  initWavefront(&obj);
  parseWavefrontObject(&obj, "./data/african_head.obj");
  for (i32 i = 0; i < obj.faceCount; i++) {
    struct WavefrontFace face = obj.faces[i];

    for (i32 j = 0; j < 3; j++) {
      struct Vec4f32 v0 = obj.vertices[face.verticesData[j].vertexIdx + 1];
      struct Vec4f32 v1 =
          obj.vertices[face.verticesData[(j + 1) % 3].vertexIdx];
      int x0 = (v0.x + 1.0f) * (WIDTH / 2.0f);
      int y0 = (v0.y + 1.0f) * (HEIGHT / 2.0f);

      int x1 = (v1.x + 1.0f) * (WIDTH / 2.0f);
      int y1 = (v1.y + 1.0f) * (HEIGHT / 2.0f);

      setLine(&image, x0, y0, x1, y1, WHITE);
    }
  }

  saveTarga(&image, "output.tga");

  destroyWavefront(&obj);
  return 0;
}
