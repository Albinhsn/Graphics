#include "files.h"
#include "lodepng.h"
#include "sdl.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parseu8FromString(u8* dest, char* source, u8* length)
{
  char number[32];
  memset(number, 0, 32);

  u8 pos = 0;
  while (isdigit(source[pos]))
  {
    pos++;
  }
  memcpy(number, source, pos);
  *dest = atoi(number);
  return pos;
}

static void parseIntFromString(int* dest, char* source, u8* length)
{
  char number[32];
  memset(number, 0, 32);

  u8 pos = 0;
  while (isdigit(source[pos]))
  {
    pos++;
  }
  memcpy(number, source, pos);
  *dest   = atoi(number);
  *length = pos;
}

static void parseFloatFromString(float* dest, char* source, u8* length)
{
  char number[32];
  u8   pos = 0;
  while (source[pos] != ' ')
  {
    pos++;
  }
  memcpy(number, source, pos);
  *dest   = atof(number);
  *length = pos;
}
void parseMapFile(struct MapFile* mapFile, char* filename)
{
  char* buffer;
  int   len;
  bool  result;
  result = read_file(&buffer, &len, filename);
  if (!result)
  {
    printf("Unable to parse map file\n");
    exit(2);
  }

  u8    curr;
  char* token = strtok(buffer, "\n");
  curr        = parseu8FromString(&mapFile->mapWidth, &token[0], &curr);
  while (!isdigit(token[curr]))
  {
    curr++;
  }
  curr += parseu8FromString(&mapFile->mapHeight, &token[curr], &curr);
  mapFile->tileIds = (u8*)malloc(sizeof(u8) * mapFile->mapWidth * mapFile->mapHeight);
  for (i32 y = 0; y < mapFile->mapHeight; y++)
  {
    curr  = 0;
    token = strtok(NULL, "\n");
    for (i32 x = 0; x < mapFile->mapWidth; x++)
    {
      curr += parseu8FromString(&mapFile->tileIds[y * mapFile->mapWidth + x], &token[curr], &curr) + 1;
    }
  }
}

void parseFontTypes(struct Font* font, const char* fileLocation)
{
  font->type = (struct FontType*)malloc(sizeof(struct FontType) * 95);

  char* buffer;
  int   len;
  read_file(&buffer, &len, fileLocation);
  FILE* file;
  file = fopen(fileLocation, "r");
  char line[256];
  char number[32];
  for (int i = 0; i < 95; i++)
  {
    memset(number, 0, 32);
    fgets(line, sizeof(line), file);
    u8 pos = 0;
    u8 inc = 0;
    // remove number
    while (line[pos] != ' ')
    {
      pos++;
    }
    // remove whitespace, number and another whitespace
    pos += 3;
    parseFloatFromString(&font->type[i].left, &line[pos], &inc);
    pos += inc;

    // This should always be the start of the number
    // parse whitespace again
    while (line[pos] == ' ')
    {
      pos++;
    }

    parseFloatFromString(&font->type[i].right, &line[pos], &inc);
    pos += inc;
    while (line[pos] == ' ')
    {
      pos++;
    }
    parseIntFromString(&font->type[i].size, &line[pos], &inc);
    font->type[i].size *= 2;
  }
}

bool parsePNG(struct Image* png, const char* filename)
{
  unsigned error;
  png->bpp = 4;

  error    = lodepng_decode32_file(&png->data, &png->width, &png->height, filename);
  if (error != 0)
  {
    printf("Failed to decode png '%s'\n", filename);
    return false;
  }

  return true;
}

bool read_file(char** buffer, int* len, const char* fileName)
{
  FILE* filePtr;
  long  fileSize, count;
  int   error;

  filePtr = fopen(fileName, "r");
  if (!filePtr)
  {
    printf("Unable to read file from '%s'\n", fileName);
    return false;
  }

  fileSize            = fseek(filePtr, 0, SEEK_END);
  fileSize            = ftell(filePtr);

  *len                = fileSize;
  *buffer             = (char*)malloc(sizeof(char) * (fileSize + 1));
  (*buffer)[fileSize] = '\0';
  fseek(filePtr, 0, SEEK_SET);
  count = fread(*buffer, 1, fileSize, filePtr);
  if (count != fileSize)
  {
    free(*buffer);
    return false;
  }

  error = fclose(filePtr);
  if (error != 0)
  {
    free(*buffer);
    return false;
  }

  return true;
}

void initSingleColorImage(struct Image* image, struct Vec3i32 color)
{

  int bpp       = 4;

  image->width  = 64;
  image->height = 64;
  image->bpp    = bpp;
  i32 size      = image->width * image->height * bpp;
  image->data   = (unsigned char*)malloc(sizeof(unsigned char*) * size);

  for (int i = 0; i < image->width * image->height * bpp; i += 4)
  {
    image->data[i + 0] = color.x;
    image->data[i + 1] = color.y;
    image->data[i + 2] = color.z;
    image->data[i + 3] = 255;
  }
}

void initUnfilledQuadImage(struct Image* image, struct Vec3i32 color, int lineThickness, u8 width, u8 height)
{

  int bpp       = 4;

  image->width  = width;
  image->height = height;
  image->bpp    = bpp;
  i32 size      = image->width * image->height * bpp;
  image->data   = (unsigned char*)malloc(sizeof(unsigned char*) * size);
  for (i32 i = 0; i < size; i++)
  {
    image->data[i] = 0;
  }

  int bottomRow = (image->height - lineThickness) * image->width * bpp;

  for (int i = 0; i < image->width * lineThickness * bpp; i += 4)
  {
    image->data[i + 0]             = color.x;
    image->data[i + 1]             = color.y;
    image->data[i + 2]             = color.z;
    image->data[i + 3]             = 255;

    image->data[bottomRow + i + 0] = color.x;
    image->data[bottomRow + i + 1] = color.y;
    image->data[bottomRow + i + 2] = color.z;
    image->data[bottomRow + i + 3] = 255;
  }

  int lastColumn = (image->width - lineThickness) * bpp;
  for (int i = 0; i < image->height; i++)
  {
    int yOffset = i * image->width * bpp;
    for (int j = 0; j < lineThickness * bpp; j += 4)
    {
      image->data[yOffset + j + 0]              = color.x;
      image->data[yOffset + j + 1]              = color.y;
      image->data[yOffset + j + 2]              = color.z;
      image->data[yOffset + j + 3]              = 255;

      image->data[yOffset + lastColumn + j + 0] = color.x;
      image->data[yOffset + lastColumn + j + 1] = color.y;
      image->data[yOffset + lastColumn + j + 2] = color.z;
      image->data[yOffset + lastColumn + j + 3] = 255;
    }
  }
}

void parseFont(struct Font* font)
{

  parsePNG(&font->image, FONT_IMAGE_LOCATION);

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &font->texture);
  glBindTexture(GL_TEXTURE_2D, font->texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font->image.width, font->image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, font->image.data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  sta_glGenerateMipmap(GL_TEXTURE_2D);

  font->spaceSize = 10;
  font->height    = 128.0f;
  parseFontTypes(font, FONT_DATA_LOCATION);
}

bool readTargaImage(struct Image* image, const char* filename)
{

  struct TargaHeader targaFileHeader;

  FILE*              filePtr;
  unsigned long      count, imageSize;
  unsigned char*     targaData;
  unsigned char*     targaImage;

  // Open the targa file for reading in binary.
  filePtr = fopen(filename, "rb");
  if (filePtr == NULL)
  {
    printf("ERROR: file doesn't exist %s\n", filename);
    return NULL;
  }

  // Read in the file header.
  count = fread(&targaFileHeader, sizeof(struct TargaHeader), 1, filePtr);
  if (count != 1)
  {
    printf("ERROR: Failed to read into header\n");
    return NULL;
  }

  // Get the important information from the header.
  image->width  = (int)targaFileHeader.width;
  image->height = (int)targaFileHeader.height;
  image->bpp    = (int)targaFileHeader.imagePixelSize;

  // Calculate the size of the 32 bit image data.
  if (image->bpp == 32)
  {
    imageSize = image->width * image->height * 4;
  }
  else if (image->bpp == 24)
  {
    imageSize = image->width * image->height * 3;
  }
  else
  {
    printf("Dont't know how to parse targa image with bpp of '%d'\n", image->bpp);
    exit(2);
  }

  // Allocate memory for the targa image data.
  targaImage = (unsigned char*)malloc(sizeof(unsigned char) * imageSize);

  // Read in the targa image data.
  count = fread(targaImage, 1, imageSize, filePtr);
  if (count != imageSize)
  {
    printf("ERROR: count read doesn't equal imageSize\n");
    return NULL;
  }

  if (fclose(filePtr) != 0)
  {
    return NULL;
  }

  targaData  = (unsigned char*)malloc(sizeof(unsigned char) * imageSize);
  bool bit32 = image->bpp == 32;

  for (int idx = 0; idx < image->height * image->width; idx++)
  {
    targaData[idx]     = targaImage[idx + 2]; // Red
    targaData[idx + 1] = targaImage[idx + 1]; // Green
    targaData[idx + 2] = targaImage[idx];     // Blue
    if (bit32)
    {
      targaData[idx + 3] = targaImage[idx + 3]; // Alpha
    }
  }

  free(targaImage);
  image->data = targaData;

  return image;
}

void loadTileImages(struct Image* tileImages, const char* filename)
{
  FILE*  filePtr;
  char*  line = NULL;
  size_t len  = 0;
  size_t read;
  u8     count = 0;

  filePtr      = fopen(filename, "r");
  if (filePtr == NULL)
  {
    printf("Unable to loadTileImages from '%s'\n", filename);
    exit(1);
  }

  while ((read = getline(&line, &len, filePtr)) != -1)
  {
    line[strlen(line) - 1] = '\0';
    parsePNG(&tileImages[count], line);
    count++;
  }
}
