#include "files.h"
#include "png.h"
#include "lodepng.h"
#include <stdint.h>
#include <zconf.h>
#include <zlib.h>

bool parsePNG(struct Image *png, const char *filename) {
  unsigned error;
  png->bpp = 4;

  error =
      lodepng_decode32_file(&png->data, &png->width, &png->height, filename);
  if (error != 0) {
    printf("Failed to decode png\n");
    return false;
  }

  return true;
}

static char *ParseCharBuffer(unsigned char *buffer, int maxLen) {
  char *str = NULL;
  int len = 0;
  while (buffer[len] != '\0' && len < maxLen) {
    len++;
  }

  if (len < maxLen) {
    str = (char *)malloc(sizeof(char) * (len + 1));
    memcpy(str, buffer, len);
    str[len] = '\0';
  } else {
    printf("Didn't find end of keyword character string\n");
  }

  return str;
}

static int CharBufferToInt(unsigned char *buffer, int length) {
  int nmbr = 0;
  for (int i = 0; i < length; i++) {
    nmbr = (nmbr << 8) | buffer[i];
  }

  return nmbr;
}

static enum PNGChunkType GetChunkType(unsigned char *buffer) {
  if (memcmp(buffer, "IHDR", 4) == 0) {
    return IHDR;
  }
  if (memcmp(buffer, "PLTE", 4) == 0) {
    return PLTE;
  }
  if (memcmp(buffer, "IDAT", 4) == 0) {
    return IDAT;
  }
  if (memcmp(buffer, "IEND", 4) == 0) {
    return IEND;
  }
  if (memcmp(buffer, "tRNS", 4) == 0) {
    return TRNS;
  }
  if (memcmp(buffer, "cHRM", 4) == 0) {
    return CHRM;
  }
  if (memcmp(buffer, "gAMA", 4) == 0) {
    return GAMA;
  }
  if (memcmp(buffer, "iCCP", 4) == 0) {
    return ICCP;
  }
  if (memcmp(buffer, "sBIT", 4) == 0) {
    return SBIT;
  }
  if (memcmp(buffer, "sRGB", 4) == 0) {
    return SRGB;
  }
  if (memcmp(buffer, "tEXt", 4) == 0) {
    return TEXT;
  }
  if (memcmp(buffer, "zTXt", 4) == 0) {
    return ZTXT;
  }
  if (memcmp(buffer, "iTXt", 4) == 0) {
    return ITXT;
  }
  if (memcmp(buffer, "bKGD", 4) == 0) {
    return BKGD;
  }
  if (memcmp(buffer, "hIST", 4) == 0) {
    return HIST;
  }
  if (memcmp(buffer, "pHYs", 4) == 0) {
    return PHYS;
  }
  if (memcmp(buffer, "sPLT", 4) == 0) {
    return SPLT;
  }
  if (memcmp(buffer, "tIME", 4) == 0) {
    return TIME;
  }

  return UNKNOWN;
}

static void InitializePNGParser(struct PNGParser *parser,
                                struct FileBuffer *buffer, struct PNG *png) {
  parser->buffer = buffer->buffer;
  parser->bufferLen = buffer->len;
  parser->bitDepth = -1;
  parser->colourType = -1;
  parser->compressionMethod = -1;
  parser->filterMethod = -1;
  parser->interfaceMethod = -1;

  png->width = -1;
  png->height = -1;

  parser->png = png;
}

bool ReadFileBinary(struct FileBuffer *fileBuffer, const char *filename) {
  FILE *file;
  file = fopen(filename, "rb");
  if (file == NULL) {
    printf("File was not found\n");
    return false;
  }
  fseek(file, 0, SEEK_END);
  fileBuffer->len = ftell(file);
  fileBuffer->buffer =
      (unsigned char *)malloc(sizeof(unsigned char) * (fileBuffer->len + 1));

  // Return the file pointer back to the beginning of the file.
  fseek(file, 0, SEEK_SET);
  int count = fread(fileBuffer->buffer, 1, fileBuffer->len, file);
  if (count != fileBuffer->len) {
    printf("Count doesn't equal fileSize?\n");
    return false;
  }
  fclose(file);

  return true;
}

bool LoadPngFile(struct PNG *png, const char *filename) {
  uint8_t *data;
  struct PNGParser *parser =
      (struct PNGParser *)malloc(sizeof(struct PNGParser));
  enum PNGChunkType type;

  struct FileBuffer buffer;
  ReadFileBinary(&buffer, filename);
  InitializePNGParser(parser, &buffer, png);

  unsigned char signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  if (memcmp(buffer.buffer, signature, 8) != 0) {
    printf("Invalid png signature got '%d %d %d %d %d %d %d %d'\n",
           signature[0], signature[1], signature[2], signature[3], signature[4],
           signature[5], signature[6], signature[7]);
    return false;
  }
  parser->position += 8;
  int start;
  while (parser->position < parser->bufferLen) {
    // Get the size of the data from the first 4 bytes
    start = parser->position;
    int dataLength = CharBufferToInt(&parser->buffer[parser->position], 4);
    parser->position += 4;

    type = GetChunkType(&parser->buffer[parser->position]);
    parser->position += 4;

    switch (type) {
    case IHDR: {
      parser->png->width =
          CharBufferToInt(&parser->buffer[parser->position], 4);
      parser->position += 4;
      parser->png->height =
          CharBufferToInt(&parser->buffer[parser->position], 4);
      parser->position += 4;
      parser->bitDepth = parser->buffer[parser->position++];
      parser->colourType = parser->buffer[parser->position++];
      parser->compressionMethod = parser->buffer[parser->position++];
      parser->filterMethod = parser->buffer[parser->position++];
      parser->interfaceMethod = parser->buffer[parser->position++];

      printf("ChunkType: IHDR\n");
      printf("\tLength: %d\n", dataLength);
      printf("\tWidth: %d, Height: %d\n", parser->png->width,
             parser->png->height);
      printf("\tBit depth: %d\n", parser->bitDepth);
      printf("\tcolourType: %d\n", parser->colourType);
      printf("\tcompressionMethod: %d\n", parser->compressionMethod);
      printf("\tfilterMethod: %d\n", parser->filterMethod);
      printf("\tinterfaceMethod: %d\n", parser->interfaceMethod);
      break;
    }
    case PLTE: {
      printf("ChunkType: PLTE\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case IDAT: {
      printf("ChunkType: IDAT\n");
      if (parser->png->height == -1 || parser->png->width == -1) {
        printf("Can't parse idat without dimensions\n");
        return false;
      }
      int bitsPerPixel = 0;
      switch (parser->colourType) {
      // greyscale
      case 0: {
        bitsPerPixel = 1;
        break;
      }
      // rgb
      case 2: {
        bitsPerPixel = 3;
        break;
      }
      // palette
      case 3: {
        bitsPerPixel = 1;
        break;
      }
      // Greyscale with alpha
      case 4: {
        bitsPerPixel = 2;
        break;
      }
      // Truecolour with alpha
      case 6: {
        bitsPerPixel = 4;
        break;
      }
      }
      // Scan lines then filter every such line
      // Filter byte should be sent to compression step
      if (parser->filterMethod == 0) {
      } else if (parser->filterMethod == 3) {
      } else if (parser->filterMethod == 4) {
      } else {
        printf("Unknown filterMethod %d\n", parser->filterMethod);
        return false;
      }
      parser->position += dataLength;
      printf("\tFiltermethod %d\n", parser->filterMethod);
      printf("\tDatalength: %d\n", dataLength);

      break;
    }
    case IEND: {
      printf("ChunkType: IEND\n");
      if (dataLength != 0) {
        printf("\tReached end with not 0 in length?\n");
        return false;
      }
      break;
    }
    case TRNS: {
      printf("ChunkType: TRNS\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case CHRM: {
      printf("ChunkType: CHRM\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case GAMA: {
      printf("ChunkType: GAMA\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case ICCP: {
      printf("ChunkType: ICCP\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case SBIT: {
      printf("ChunkType: SBIT\n");
      printf("\tLength: %d\n", dataLength);
      // if (parser->colourType == -1) {
      //   printf("Can't parse SBIT without colour type\n");
      //   return false;
      // }
      if (parser->colourType == 0 && dataLength == 1) {
        printf("\tgreyscale bits: %d\n", parser->buffer[parser->position++]);
      } else if (parser->colourType == 2 && dataLength == 3) {
        printf("\tred bits: %d\n", parser->buffer[parser->position++]);
        printf("\tgreen bits: %d\n", parser->buffer[parser->position++]);
        printf("\tblue bits: %d\n", parser->buffer[parser->position++]);
      } else if (parser->colourType == 3 && dataLength == 3) {
        // palette
        printf("\tred bits: %d\n", parser->buffer[parser->position++]);
        printf("\tgreen bits: %d\n", parser->buffer[parser->position++]);
        printf("\tblue bits: %d\n", parser->buffer[parser->position++]);
      } else if (parser->colourType == 4 && dataLength == 2) {
        printf("\tgreyscale bits: %d\n", parser->buffer[parser->position++]);
        printf("\talpha bits: %d\n", parser->buffer[parser->position++]);
      } else if (parser->colourType == 6 && dataLength == 4) {
        printf("\tred bits: %d\n", parser->buffer[parser->position++]);
        printf("\tgreen bits: %d\n", parser->buffer[parser->position++]);
        printf("\tblue bits: %d\n", parser->buffer[parser->position++]);
        printf("\talpha bits: %d\n", parser->buffer[parser->position++]);
      } else {
        printf("\tUnknown colourType '%d'and dataLength '%d'\n",
               parser->colourType, dataLength);
        return false;
      }
      break;
    }
    case SRGB: {
      printf("ChunkType: SRGB\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case TEXT: {
      printf("ChunkType: TEXT\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case ZTXT: {
      printf("ChunkType: ZTXT\n");
      int start = parser->position;
      char *keyword = ParseCharBuffer(&parser->buffer[parser->position],
                                      parser->bufferLen - parser->position);

      if (!keyword) {
        printf("Didn't find end of keyword character string\n");
        return false;
      }
      parser->position += strlen(keyword);
      unsigned int compMethod = parser->buffer[parser->position++];
      printf("\tLength: %d\n", dataLength);
      printf("\tKeyword: %s\n", keyword);
      printf("\tCompMethod: %d\n", compMethod);

      int dataStreamLen = start + dataLength - parser->position;
      char *dataStream = (char *)malloc(sizeof(char) * (dataStreamLen + 1));
      memcpy(dataStream, &parser->buffer[parser->position], dataStreamLen);
      dataStream[dataStreamLen] = '\0';
      printf("\tDatastream: %s\n", dataStream);
      parser->position += 4 + dataStreamLen;
      continue;
    }
    case ITXT: {
      int start = parser->position;
      printf("ChunkType: ITXT\n");
      printf("\tLength: %d\n", dataLength);
      char *keyword = ParseCharBuffer(&parser->buffer[parser->position],
                                      parser->bufferLen - parser->position);

      if (!keyword) {
        printf("Didn't find end of keyword character string\n");
        return false;
      }
      parser->position += strlen(keyword);

      unsigned int compFlag = parser->buffer[parser->position++];
      unsigned int compMethod = parser->buffer[parser->position++];
      char *tag = ParseCharBuffer(&parser->buffer[parser->position],
                                  parser->bufferLen - parser->position);
      if (!tag) {
        printf("Didn't find end of tag character string\n");
        return false;
      }
      parser->position += strlen(tag);

      char *translatedKeyword =
          ParseCharBuffer(&parser->buffer[parser->position],
                          parser->bufferLen - parser->position);
      if (!translatedKeyword) {
        printf("Didn't find end of tag character string\n");
        return false;
      }
      parser->position += strlen(translatedKeyword);
      int textLen = start + dataLength - parser->position;
      char *text = (char *)malloc(sizeof(char) * (textLen + 1));
      memcpy(text, &parser->buffer[parser->position], textLen);
      text[textLen] = '\0';

      parser->position += textLen;

      printf("\tKeyword: %s\n", keyword);
      printf("\tCompFlag: %d\n", compFlag);
      printf("\tCompMethod: %d\n", compMethod);
      printf("\tTag: %s\n", tag);
      printf("\tTranslated keyword: %s\n", translatedKeyword);
      printf("\tText: %s\n", text);
      break;
    }
    case BKGD: {
      printf("ChunkType: BKGD\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case HIST: {
      printf("ChunkType: HIST\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case PHYS: {
      printf("ChunkType: PHYS\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case SPLT: {
      printf("ChunkType: SPLT\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case TIME: {
      printf("ChunkType: TIME\n");
      printf("\tLength: %d\n", dataLength);
      printf("Don't know how to handle\n");
      return false;
      break;
    }
    case UNKNOWN: {
      printf("Unknown format '%.*s'\n", 4, &parser->buffer[parser->position]);
      printf("\tLength: %d\n", dataLength);
      return false;
    }
    }
    uLong crc_result = crc32(crc32(0xedb88320L, Z_NULL, 0),
                             &parser->buffer[start + 4], dataLength + 4);
    uLong crc_idk = (((unsigned)parser->buffer[parser->position] << 24u) |
                     ((unsigned)parser->buffer[parser->position + 1] << 16u) |
                     ((unsigned)parser->buffer[parser->position + 2] << 8u) |
                     (unsigned)parser->buffer[parser->position + 3]);
    printf("\tCRC: %ld - %ld\n", crc_result, crc_idk);
    if (crc_idk != crc_result) {
      printf("CRC isn't correct\n");
      return false;
    }
    parser->position += 4;
  }

  return true;
}
