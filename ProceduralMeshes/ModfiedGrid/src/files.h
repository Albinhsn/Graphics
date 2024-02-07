#ifndef IMAGE_H
#define IMAGE_H
#include "string.h"
#include <stdbool.h>
#include <stdio.h>

struct Image {
  unsigned int width, height;
  unsigned int bpp;
  unsigned char *data;
};
struct TargaHeader {
  unsigned char data1[12];
  unsigned short width;
  unsigned short height;
  unsigned char bpp;
  unsigned char data2;
};

struct Image *LoadTarga(const char *filename);
bool ah_ReadFile(struct String *string, const char *fileName);

char *ah_strcpy(char *buffer, struct String *s2);

#endif
