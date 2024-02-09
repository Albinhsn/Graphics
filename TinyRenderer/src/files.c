#include "files.h"
#include "image.h"
#include "vector.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void debugTargaHeader(struct TargaHeader header)
{
  for (int i = 0; i < 18; i++)
  {
    printf("%d ", header.header[i]);
  }
  printf("\n");
}

bool loadTarga(struct Image* image, const char* filename)
{
  printf("Loading targa '%s'\n", filename);

  struct TargaHeader header;

  FILE*              filePtr;
  unsigned long      count, imageSize;

  // Open the targa file for reading in binary.
  filePtr = fopen(filename, "rb");
  if (filePtr == NULL)
  {
    printf("ERROR: file doesn't exist %s\n", filename);
    return false;
  }

  // Read in the file header.
  count = fread(&header, sizeof(struct TargaHeader), 1, filePtr);
  if (count != 1)
  {
    printf("ERROR: Failed to read into header\n");
    return false;
  }
  debugTargaHeader(header);

  if (header.imagePixelSize == 32)
  {
    imageSize = header.width * header.height * 4;
    printf("32 bpp\n");
  }
  else if (header.imagePixelSize == 24)
  {
    imageSize = header.width * header.height * 4;
    printf("24 bpp\n");
  }
  else
  {
    printf("Dont't know how to parse targa image with bpp of '%d'\n", header.imagePixelSize);
    exit(2);
  }
  ui8 bpp = 4;

  // Allocate memory for the targa image data.
  image->data = (ui8*)malloc(sizeof(ui8) * imageSize);
  for (i32 i = 0; i < 0; i++)
  {
    image->data[i] = 0;
  }
  image->width  = header.width;
  image->height = header.height;
  printf("Got dimensions: %d %d\n", image->width, image->height);

  if (header.imageType == 2)
  {
    // Read in the targa image data.
    count = fread(image->data, 1, imageSize, filePtr);
    if (count != imageSize)
    {
      printf("ERROR: count read doesn't equal imageSize %ld %ld\n", count, imageSize);
      return false;
    }
  }
  else if (header.imageType == 10)
  {
    if (header.imagePixelSize == 24)
    {
      i32 imageIndex = 0;
      ui8 byte;
      while (imageIndex < imageSize)
      {
        fread(&byte, 1, sizeof(ui8), filePtr);

        ui8* curr = &image->data[imageIndex];
        if (curr[0] != 0)
        {
          printf("Overwrite at %d\n", imageIndex);
        }
        if (byte >= 128)
        {
          ui8            repeated = byte - 127;

          struct Vec3ui8 color;
          fread(&color, 1, sizeof(struct Vec3ui8), filePtr);

          for (i32 j = 0; j < repeated; j++)
          {
            curr[j * bpp + 0] = color.x;
            curr[j * bpp + 1] = color.y;
            curr[j * bpp + 2] = color.z;
            curr[j * bpp + 3] = 255;
          }
          imageIndex += repeated * bpp;
        }
        else
        {
          ui8 repeated = byte + 1;

          for (i32 j = 0; j < repeated; j++)
          {
            struct Vec3ui8 color;
            fread(&color, 1, sizeof(struct Vec3ui8), filePtr);

            curr[j * bpp + 0] = color.x;
            curr[j * bpp + 1] = color.y;
            curr[j * bpp + 2] = color.z;
            curr[j * bpp + 3] = 255;
          }
          imageIndex += repeated * bpp;
        }
      }
    }
    else
    {
      i32 imageIndex = 0;
      ui8 byte;
      while (imageIndex < imageSize)
      {
        fread(&byte, 1, sizeof(ui8), filePtr);

        ui8* curr = &image->data[imageIndex];
        if (curr[0] != 0)
        {
          printf("Overwrite at %d\n", imageIndex);
        }
        if (byte >= 128)
        {
          ui8            repeated = byte - 127;

          struct Vec4ui8 color;
          fread(&color, 1, sizeof(struct Vec4ui8), filePtr);

          for (i32 j = 0; j < repeated; j++)
          {
            curr[j * bpp + 0] = color.x;
            curr[j * bpp + 1] = color.y;
            curr[j * bpp + 2] = color.z;
            curr[j * bpp + 3] = color.w;
          }
          imageIndex += repeated * bpp;
        }
        else
        {
          ui8 repeated = byte + 1;

          for (i32 j = 0; j < repeated; j++)
          {
            struct Vec4ui8 color;
            fread(&color, 1, sizeof(struct Vec4ui8), filePtr);

            curr[j * bpp + 0] = color.x;
            curr[j * bpp + 1] = color.y;
            curr[j * bpp + 2] = color.z;
            curr[j * bpp + 3] = color.w;
          }
          imageIndex += repeated * bpp;
        }
      }
    }
  }

  if (fclose(filePtr) != 0)
  {
    printf("Failed to close file\n");
    return false;
  }
  return true;
}

static ui8 skipWhitespace(const char* line)
{
  ui8 curr = 0;
  while (curr < strlen(line) && !(isdigit(line[curr]) || (line[curr] == '-')))
  {
    curr++;
  }
  return curr;
}

static i32 parseIntFromString(const char* source, ui8* length)
{
  char number[32];
  memset(number, 0, 32);

  ui8 pos = 0;
  while (pos < strlen(source) && isdigit(source[pos]))
  {
    pos++;
  }
  memcpy(number, source, pos);
  *length = pos;
  return atoi(number);
}

static f32 parseFloatFromString(const char* source, ui8* length)
{
  char number[32];
  ui8  pos = 0;

  while (pos < strlen(source) && source[pos] != ' ')
  {
    pos++;
  }
  memcpy(number, source, pos);
  *length = pos;
  return atof(number);
}

void initWavefront(struct WavefrontObject* obj)
{
  ui32 initCap                   = 8;
  obj->vertexCount               = 0;
  obj->vertices                  = (struct Vec4f32*)malloc(sizeof(struct Vec4f32) * initCap);
  obj->vertexCapacity            = initCap;

  obj->normalCount               = 0;
  obj->normals                   = (struct Vec3f32*)malloc(sizeof(struct Vec3f32) * initCap);
  obj->normalCapacity            = initCap;

  obj->textureCoordinateCount    = 0;
  obj->textureCoordinates        = (struct Vec3f32*)malloc(sizeof(struct Vec3f32) * initCap);
  obj->textureCoordinateCapacity = initCap;

  obj->faceCount                 = 0;
  obj->faceCapacity              = initCap;
  obj->faces                     = (struct WavefrontFace*)malloc(sizeof(struct WavefrontFace) * initCap);
}

void destroyWavefront(struct WavefrontObject* obj)
{
  free(obj->textureCoordinates);
  free(obj->vertices);
  free(obj->normals);
  for (i32 i = 0; i < obj->faceCount; i++)
  {
    struct WavefrontFace face = obj->faces[i];
    free(face.verticesData);
  }
  free(obj->faces);
}

static inline void resizeTextureCoordinates(struct WavefrontObject* obj)
{
  if (obj->textureCoordinateCount >= obj->textureCoordinateCapacity)
  {
    obj->textureCoordinateCapacity *= 2;
    obj->textureCoordinates = realloc(obj->textureCoordinates, sizeof(struct Vec3f32) * obj->textureCoordinateCapacity);
  }
}

static void parseWavefrontTexture(struct WavefrontObject* obj, const char* line)
{
  ui8 step;
  ui8 curr = 2 + skipWhitespace(&line[2]);
  obj->textureCoordinateCount++;
  resizeTextureCoordinates(obj);

  for (i32 i = 0; i < 2; i++)
  {
    obj->textureCoordinates[obj->textureCoordinateCount - 1].pos[i] = parseFloatFromString(&line[curr], &step);
    curr += step;
    curr += skipWhitespace(&line[curr]);
  }

  // Parse optional w
  if (curr < strlen(line))
  {
    obj->textureCoordinates[obj->textureCoordinateCount - 1].z = parseFloatFromString(line, &step);
  }
  else
  {
    obj->textureCoordinates[obj->textureCoordinateCount - 1].z = 0;
  }
}

static inline void resizeNormals(struct WavefrontObject* obj)
{
  if (obj->normalCount >= obj->normalCapacity)
  {
    obj->normalCapacity *= 2;
    obj->normals = realloc(obj->normals, sizeof(struct Vec3f32) * obj->normalCapacity);
  }
}
static void parseWavefrontNormal(struct WavefrontObject* obj, const char* line)
{

  ui8 step;
  ui8 curr = 2 + skipWhitespace(&line[2]);
  obj->normalCount++;
  resizeNormals(obj);

  // Parse x,y,z
  for (i32 i = 0; i < 3; i++)
  {
    obj->normals[obj->normalCount - 1].pos[i] = parseFloatFromString(&line[curr], &step);
    curr += step;
    curr += skipWhitespace(&line[curr]);
  }
}

static inline void resizeVertices(struct WavefrontObject* obj)
{
  if (obj->vertexCount >= obj->vertexCapacity)
  {
    obj->vertexCapacity *= 2;
    obj->vertices = realloc(obj->vertices, sizeof(struct Vec4f32) * obj->vertexCapacity);
  }
}

static void parseWavefrontVertex(struct WavefrontObject* obj, const char* line)
{
  ui8 step;
  ui8 curr = 1 + skipWhitespace(&line[1]);
  obj->vertexCount++;
  resizeVertices(obj);

  // Parse x,y,z
  for (i32 i = 0; i < 3; i++)
  {
    obj->vertices[obj->vertexCount - 1].pos[i] = parseFloatFromString(&line[curr], &step);
    curr += step;
    curr += skipWhitespace(&line[curr]);
  }

  // Parse optional w
  if (curr < strlen(line))
  {
    obj->vertices[obj->vertexCount - 1].w = parseFloatFromString(line, &step);
  }
  else
  {
    obj->vertices[obj->vertexCount - 1].w = 0;
  }
}

static inline void resizeFaces(struct WavefrontObject* obj)
{
  if (obj->faceCount >= obj->faceCapacity)
  {
    obj->faceCapacity *= 2;
    obj->faces = realloc(obj->faces, sizeof(struct Vec4f32) * obj->faceCapacity);
  }
}

static inline void resizeFaceVertexData(struct WavefrontFace* face, ui32* cap)
{
  if (face->vertexCount >= *cap)
  {
    *cap *= 2;
    face->verticesData = realloc(face->verticesData, *cap);
  }
}

static void parseWavefrontFace(struct WavefrontObject* obj, const char* line)
{
  ui8 step;
  ui8 curr = 1 + skipWhitespace(&line[1]);
  obj->faceCount++;
  resizeFaces(obj);

  struct WavefrontFace* face      = &obj->faces[obj->faceCount - 1];
  ui32                  vertexCap = 8;
  face->vertexCount               = 0;
  face->verticesData              = (struct VertexData*)malloc(sizeof(struct VertexData) * vertexCap);
  while (curr < strlen(line))
  {
    face->vertexCount++;
    resizeFaceVertexData(face, &vertexCap);

    face->verticesData[face->vertexCount - 1].vertexIdx = parseIntFromString(&line[curr], &step);
    curr += step;
    curr++;
    face->verticesData[face->vertexCount - 1].textureIdx = parseIntFromString(&line[curr], &step);
    curr += step;
    curr++;
    face->verticesData[face->vertexCount - 1].normalIdx = parseIntFromString(&line[curr], &step);

    curr += step + skipWhitespace(&line[curr]);
    curr++;
  }
}

static inline void parseWavefrontLine(struct WavefrontObject* obj, const char* line)
{
  if (line[0] == 'v' && line[1] == 't')
  {
    parseWavefrontTexture(obj, line);
  }
  else if (line[0] == 'v' && line[1] == 'n')
  {
    parseWavefrontNormal(obj, line);
  }
  else if (line[0] == 'v')
  {
    parseWavefrontVertex(obj, line);
  }
  else if (line[0] == 'f')
  {
    parseWavefrontFace(obj, line);
  }
  else
  {
    printf("Don't know how to parse '%s'\n", line);
  }
}
void parseWavefrontObject(struct WavefrontObject* obj, const char* filename)
{
  FILE* filePtr;
  char* line = NULL;
  ui64  len  = 0;
  i64   read;

  filePtr = fopen(filename, "r");
  if (filePtr == NULL)
  {
    printf("Unable to loadTileImages from '%s'\n", filename);
    exit(1);
  }

  while ((read = getline(&line, &len, filePtr)) != -1)
  {
    line[strlen(line) - 1] = '\0';
    parseWavefrontLine(obj, line);
  }
}

void saveTarga(struct Image* image, const char* filename)
{
  printf("Saving at '%s'\n", filename);
  FILE*              filePtr;
  unsigned long      count;

  struct TargaHeader h;
  memset(&h.header[0], 0, 18);
  h.width          = image->width;
  h.height         = image->height;
  h.imageType      = 2;
  h.imagePixelSize = 32;

  filePtr          = fopen(filename, "wb");
  count            = fwrite(&h, sizeof(struct TargaHeader), 1, filePtr);
  if (count != 1)
  {
    printf("ERROR: Failed to write into header\n");
    return;
  }

  ui32 imageSize = 4 * image->width * image->height;

  // Read in the targa image data.
  count = fwrite(image->data, 1, imageSize, filePtr);
  if (count != imageSize)
  {
    printf("ERROR: count after write doesn't equal imageSize %ld %d\n", count, imageSize);
    return;
  }

  if (fclose(filePtr) != 0)
  {
    printf("Failed to close file\n");
    return;
  }
}
