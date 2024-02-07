#ifndef FILES_H

#include "common.h"
#include "image.h"
#include "vector.h"
#include <stdbool.h>

struct VertexData {
  ui32 vertexIdx;
  ui32 textureIdx;
  ui32 normalIdx;
};

struct WavefrontFace {
  struct VertexData *verticesData;
  ui32 vertexCount;
};

struct WavefrontObject {
  ui32 vertexCount;
  ui32 vertexCapacity;
  struct Vec4f32 *vertices;
  ui32 textureCoordinateCount;
  ui32 textureCoordinateCapacity;
  struct Vec3f32 *textureCoordinates;
  ui32 normalCount;
  ui32 normalCapacity;
  struct Vec3f32 *normals;
  ui32 faceCount;
  ui32 faceCapacity;
  struct WavefrontFace *faces;
};

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

bool loadTarga(struct Image *image, const char *filename);
void saveTarga(struct Image *image, const char *filename);
void destroyWavefront(struct WavefrontObject *obj);
void parseWavefrontObject(struct WavefrontObject *obj, const char *filename);
void initWavefront(struct WavefrontObject *obj);
#endif
