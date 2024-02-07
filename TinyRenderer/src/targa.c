#include "targa.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool loadTarga(struct TargaImage *image, const char *filename) {

  FILE *filePtr;
  unsigned long count, imageSize;

  // Open the targa file for reading in binary.
  filePtr = fopen(filename, "rb");
  if (filePtr == NULL) {
    printf("ERROR: file doesn't exist %s\n", filename);
    return false;
  }

  // Read in the file header.
  count = fread(&image->targaHeader, sizeof(struct TargaHeader), 1, filePtr);
  if (count != 1) {
    printf("ERROR: Failed to read into header\n");
    return false;
  }

  if (image->targaHeader.imagePixelSize == 32) {
    imageSize = image->targaHeader.width * image->targaHeader.height * 4;
  } else if (image->targaHeader.imagePixelSize == 24) {
    imageSize = image->targaHeader.width * image->targaHeader.height * 3;
  } else {
    printf("Dont't know how to parse targa image with bpp of '%d'\n",
           image->targaHeader.imagePixelSize);
    exit(2);
  }

  // Allocate memory for the targa image data.
  image->data = (ui8 *)malloc(sizeof(ui8) * imageSize);

  // Read in the targa image data.
  count = fread(image->data, 1, imageSize, filePtr);
  if (count != imageSize) {
    printf("ERROR: count read doesn't equal imageSize\n");
    return false;
  }

  if (fclose(filePtr) != 0) {
    printf("Failed to close file\n");
    return false;
  }
  return true;
}
static inline void setPixel(struct TargaImage *image, ui16 x, ui16 y,
                            struct Vec4ui8 color) {
  i32 idxOffset = image->withInPixels + x * 4;
  image->data[idxOffset + 0] = color.b;
  image->data[idxOffset + 1] = color.g;
  image->data[idxOffset + 2] = color.r;
  image->data[idxOffset + 3] = color.a;
}

static inline i16 absDiff(i16 x, i16 y) {
  i16 diff = x - y;
  return diff < 0 ? -diff : diff;
}

static inline i16 absi16(i16 x) { return x < 0 ? -x : x; }

void setLine(struct TargaImage *image, ui16 x0, ui16 y0, ui16 x1, ui16 y1,
             struct Vec4ui8 color) {

  bool steep = absi16(x0 - x1) < absi16(y0 - y1);

  if (steep) {
    i16 tmp = x0;
    x0 = y0;
    y0 = tmp;

    tmp = x1;
    x1 = y1;
    y1 = tmp;
  }

  if (x0 > x1) {
    i16 tmp = x0;
    x0 = x1;
    x1 = tmp;

    tmp = y0;
    y0 = y1;
    y1 = tmp;
  }

  i16 dx = x1 - x0;
  i16 dy = y1 - y0;

  i16 d = 2 * dy;
  bool downwards = d < 0;
  d = downwards ? -d : d;

  i16 derror = 0;
  i16 y = y0;
  for (i16 x = x0; x <= x1; x++) {
    setPixel(image, steep ? y : x, steep ? x : y, color);
    derror += d;
    if (derror > dx) {
      y = downwards ? y - 1 : y + 1;
      derror -= 2 * dx;
    }
  }
}

void initTargaImage(struct TargaImage *image, ui16 width, ui16 height,
                    ui8 *data) {
  for (int i = 0; i < 18; i++) {
    image->targaHeader.header[i] = 0;
  }
  image->targaHeader.imageType = 2;
  image->targaHeader.width = width;
  image->targaHeader.height = height;
  image->targaHeader.imagePixelSize = 32;

  for (i32 i = 0; i < width * height * 4; i += 4) {
    data[i + 0] = 0;
    data[i + 1] = 0;
    data[i + 2] = 0;
    data[i + 3] = 0xFF;
  }

  image->withInPixels = image->targaHeader.width * 4;
  image->data = data;
}

void debugTargaData(struct TargaImage *image) {
  i32 pixelRowLength = image->targaHeader.width * 4;
  for (i32 y = 0; y < image->targaHeader.height; y++) {
    for (int x = 0; x < pixelRowLength; x += 4) {
      printf("|%d %d %d %d", image->data[y * pixelRowLength + x + 0],
             image->data[y * pixelRowLength + x + 1],
             image->data[y * pixelRowLength + x + 2],
             image->data[y * pixelRowLength + x + 3]);
    }
    printf("|\n");
  }
}

void saveTarga(struct TargaImage *image, const char *filename) {

  FILE *filePtr;
  unsigned long count;

  filePtr = fopen(filename, "wb");
  struct TargaHeader h = image->targaHeader;
  count = fwrite(&image->targaHeader, sizeof(struct TargaHeader), 1, filePtr);
  if (count != 1) {
    printf("ERROR: Failed to write into header\n");
    return;
  }

  ui32 imageSize = 4 * image->targaHeader.width * image->targaHeader.height;

  // Read in the targa image data.
  count = fwrite(image->data, 1, imageSize, filePtr);
  if (count != imageSize) {
    printf("ERROR: count after write doesn't equal imageSize\n");
    return;
  }

  if (fclose(filePtr) != 0) {
    printf("Failed to close file\n");
    return;
  }
}
