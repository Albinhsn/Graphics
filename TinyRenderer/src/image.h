#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"
struct Image {
  ui16 width;
  ui16 height;
  ui8 *data;
};

void setLine(struct Image *image, ui16 x0, ui16 y0, ui16 x1, ui16 y1,
             struct Vec4ui8 color);
void initImage(struct Image *image, ui16 width, ui16 height, ui8 *data);
void debugImageData(struct Image *image);

#endif
