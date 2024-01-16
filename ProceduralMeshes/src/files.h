#include "string.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct Image{
  int width, height;
  int bpp;
  unsigned char * data;
};
struct TargaHeader {
    unsigned char data1[12];
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char data2;
};

struct Image *LoadTarga(const char *filename);
struct String *ah_ReadFile(char *fileName);
char *ah_strcpy(char *buffer, struct String *s2);
