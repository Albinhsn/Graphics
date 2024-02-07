#ifndef TARGA_H

#include "common.h"
#include "vector.h"
#include <stdbool.h>

struct TargaHeader {
  union {
    ui8 header[18];
    struct {
      ui8 charactersInIdentificationField;
      ui8 colorMapType;
      ui8 imageType;
      ui8 colorMapSpec[5];
      ui16 xOrigin;
      ui16 yOrigin;
      ui16 width;
      ui16 height;
      ui8 imagePixelSize;
      ui8 imageDescriptor;
    };
  };
};

struct TargaImage {
  struct TargaHeader targaHeader;
  ui8 *data;
};

bool loadTarga(struct TargaImage *image, const char *filename);
void saveTarga(struct TargaImage *image, const char *filename);
void initTargaImage(struct TargaImage *image, ui16 width, ui16 height,
                    ui8 *data);

void setPixel(struct TargaImage *image, ui16 x, ui16 y, struct Vec4ui8 color);
void debugTargaData(struct TargaImage *image);
#endif
