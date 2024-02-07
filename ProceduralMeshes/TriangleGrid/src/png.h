#ifndef STA_PNG_H
#define STA_PNG_H

#include "string.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct PNG {
  unsigned int width;
  unsigned int height;
  unsigned char *data;
  unsigned int bpp;
};

enum PNGChunkType {
  // Critical chunk
  IHDR,
  PLTE,
  IDAT,
  IEND,
  // Ancillary chunks
  TRNS,
  CHRM,
  GAMA,
  ICCP,
  SBIT,
  SRGB,
  TEXT,
  ZTXT,
  ITXT,
  BKGD,
  HIST,
  PHYS,
  SPLT,
  TIME,
  UNKNOWN
};

struct PNGParser {
  unsigned int position;
  unsigned char *buffer;
  int bufferLen;
  struct PNG *png;
  unsigned int bitDepth;
  uint8_t colourType;
  uint8_t compressionMethod;
  uint8_t filterMethod;
  uint8_t interfaceMethod;
};
bool parsePNG(struct Image *png, const char *filename);
bool ReadFileBinary(struct FileBuffer *fileBuffer, const char *filename);
bool LoadPngFile(struct PNG *png, const char *filename);
#endif
