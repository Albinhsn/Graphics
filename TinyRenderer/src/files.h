#ifndef FILES_H

#include "common.h"
#include "image.h"
#include "vector.h"
#include <stdbool.h>

struct VertexData {
  u32 vertexIdx;
  u32 textureIdx;
  u32 normalIdx;
};

struct WavefrontFace {
  struct VertexData *verticesData;
  u32 vertexCount;
};

struct WavefrontObject {
  u32 vertexCount;
  u32 vertexCapacity;
  struct Vec4f32 *vertices;
  u32 textureCoordinateCount;
  u32 textureCoordinateCapacity;
  struct Vec3f32 *textureCoordinates;
  u32 normalCount;
  u32 normalCapacity;
  struct Vec3f32 *normals;
  u32 faceCount;
  u32 faceCapacity;
  struct WavefrontFace *faces;
};

struct TargaHeader {
  union {
    u8 header[18];
    struct {
      u8 charactersInIdentificationField;
      u8 colorMapType;
      u8 imageType;
      u8 colorMapSpec[5];
      u16 xOrigin;
      u16 yOrigin;
      u16 width;
      u16 height;
      u8 imagePixelSize;
      u8 imageDescriptor;
    };
  };
};

bool loadTarga(struct Image *image, const char *filename);
void saveTarga(struct Image *image, const char *filename);
void destroyWavefront(struct WavefrontObject *obj);
void parseWavefrontObject(struct WavefrontObject *obj, const char *filename);
void initWavefront(struct WavefrontObject *obj);
#endif
