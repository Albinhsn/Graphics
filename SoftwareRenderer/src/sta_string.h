#ifndef STA_STRING_H
#define STA_STRING_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
struct String
{
  uint64_t len;
  uint64_t capacity;
  char*    buffer;
};

struct FileBuffer
{
  uint64_t       len;
  unsigned char* buffer;
};

#endif
