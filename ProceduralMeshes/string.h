

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
struct String {
  uint64_t len;
  char *buffer;
};

struct FileBuffer {
  uint64_t len;
  unsigned char *buffer;
};
