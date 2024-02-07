#ifndef TARGA_H 


#include "common.h"
#include <stdbool.h>

struct TargaHeader {
  ui8 data1[12];
  ui16 width;
  ui16 height;
  ui8 bpp;
  ui8 data2;
};

struct TargaImage {
  struct TargaHeader targaHeader;
  ui8 *data;
};

bool loadTarga(struct TargaImage *image, const char *filename);
void saveTarga(struct TargaImage *image, const char *filename);

#endif 
