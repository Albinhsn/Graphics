#ifndef IMAGE_H
#define IMAGE_H
#include "vector.h"
#include <GL/gl.h>
#include <stdbool.h>

#define IDLE_STATE     0
#define RUNNING_STATE  1
#define ATTACK_1_STATE 2
#define ATTACK_2_STATE 3
#define ATTACK_3_STATE 4

struct PNG
{
  unsigned int   width;
  unsigned int   height;
  unsigned char* data;
  unsigned int   bpp;
};

struct Image
{
  unsigned int   width, height;
  unsigned int   bpp;
  unsigned char* data;
};
typedef struct Image Image;

struct BufferData
{
  struct Vec3f32 vertices;
  struct Vec3f32 indices;
};

struct Mesh
{
  int                bufferDatalength;
  struct BufferData* bufferData;
};

struct Texture
{
  GLuint textureUnit;
  GLuint textureId;
};
typedef struct Texture Texture;

struct FontType
{
  float left, right;
  int   size;
};
typedef struct FontType FontType;

struct Font
{
  struct FontType* type;
  struct Image     image;
  GLuint            program;
  GLuint            texture;
  GLuint            vertexArrayId;
  GLuint            vertexBufferId;
  float            height;
  int              spaceSize;
};
typedef struct Font Font;

struct TargaHeader
{
  union
  {
    u8 header[18];
    struct
    {
      u8  charactersInIdentificationField;
      u8  colorMapType;
      u8  imageType;
      u8  colorMapSpec[5];
      u16 xOrigin;
      u16 yOrigin;
      u16 width;
      u16 height;
      u8  imagePixelSize;
      u8  imageDescriptor;
    };
  };
};

// struct TargaHeader
// {
//   unsigned char  data1[12];
//   unsigned short width;
//   unsigned short height;
//   unsigned char  bpp;
//   unsigned char  data2;
// };

struct MapFile
{
  u8  mapWidth;
  u8  mapHeight;
  u8* tileIds;
};

void parseMapFile(struct MapFile* mapFile, char* filename);
void parseFontTypes(struct Font* font, const char* fileLocation);
void initSingleColorImage(struct Image* image, struct Vec3i32 color);
void parseFont(struct Font* font);
bool readTargaImage(struct Image* image, const char* filename);
bool read_file(char** buffer, int* len, const char* fileName);
void initUnfilledQuadImage(struct Image* image, struct Vec3i32 color, int lineThickness, u8 width, u8 height);
bool parsePNG(struct Image* png, const char* filename);
void loadTileImages(struct Image* tileImages, const char* filename);

#endif
