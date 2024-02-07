#include "targa.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool loadTarga(struct TargaImage *image, const char *filename) {

  FILE *filePtr;
  unsigned long count, imageSize;
  unsigned char *targaData;
  unsigned char *targaImage;

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

  if (image->targaHeader.bpp == 32) {
    imageSize = image->targaHeader.width * image->targaHeader.height * 4;
  } else if (image->targaHeader.bpp == 24) {
    imageSize = image->targaHeader.width * image->targaHeader.height * 3;
  } else {
    printf("Dont't know how to parse targa image with bpp of '%d'\n",
           image->targaHeader.bpp);
    exit(2);
  }

  // Allocate memory for the targa image data.
  image->data = (unsigned char *)malloc(sizeof(unsigned char) * imageSize);

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
void saveTarga(struct TargaImage *image, const char *filename) {

  FILE *filePtr;
  unsigned long count;
  ui8 *targaData;
  ui8 *targaImage;

  filePtr = fopen(filename, "w");

  count = fwrite(&image->targaHeader, sizeof(struct TargaHeader), 1, filePtr);
  if (count != 1) {
    printf("ERROR: Failed to write into header\n");
    return;
  }

  ui32 imageSize = (image->targaHeader.bpp == 32 ? 4 : 3) *
                   image->targaHeader.width * image->targaHeader.height;

  // Read in the targa image data.
  count = fwrite(image->data, 1, imageSize, filePtr);
  if (count != imageSize) {
    printf("ERROR: count after write doesn't equal imageSize\n");
    return;
  }

  if (fclose(filePtr) != 0) {
    return;
  }
}
