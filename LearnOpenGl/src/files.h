#ifndef STA_FILES_H
#define STA_FILES_H
#include "common.h"
#include "string.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>

struct TargaImage
{
  u64            width, height;
  i32            bpp;
  unsigned char* data;
};
typedef struct TargaImage TargaImage;

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
typedef struct TargaHeader TargaHeader;

struct CSVRecord
{
  Buffer* data;
  u64     dataCount;
  u64     dataCap;
};
typedef struct CSVRecord CSVRecord;

struct CSV
{
  CSVRecord* records;
  u64        recordCount;
  u64        recordCap;
};
typedef struct CSV CSV;

enum JsonType
{
  JSON_VALUE,
  JSON_OBJECT,
  JSON_ARRAY,
  JSON_STRING,
  JSON_NUMBER,
  JSON_BOOL,
  JSON_NULL
};
typedef enum JsonType JsonType;

struct JsonValue;
struct JsonObject;
struct JsonArray;

struct JsonValue
{
  JsonType type;
  union
  {
    struct JsonObject* obj;
    struct JsonArray*  arr;
    bool               b;
    char*              string;
    float              number;
  };
};
typedef struct JsonValue JsonValue;

struct JsonObject
{
  char**     keys;
  JsonValue* values;
  u64        size;
  u64        cap;
};
typedef struct JsonObject JsonObject;

struct JsonArray
{
  uint32_t   arraySize;
  uint32_t   arrayCap;
  JsonValue* values;
};
typedef struct JsonArray JsonArray;

struct Json
{
  JsonType headType;
  union
  {
    JsonValue  value;
    JsonObject obj;
    JsonArray  array;
  };
};
typedef struct Json Json;

bool                sta_deserialize_json_from_file(Arena* arena, Json* json, const char* filename);
bool                sta_serialize_json_to_file(Json* json, const char* filename);
void                sta_debug_json(Json* json);

bool                sta_read_csv_from_file(Arena* arena, CSV* csv, const char* filelocation);
bool                sta_write_csv_to_file(CSV* csv, const char* filelocation);
void                sta_debug_csv(CSV* csv);
bool                sta_read_csv_from_string(CSV* csv, const char* csv_data);
void                sta_targa_write(TargaImage* image, const char* filename);

bool                sta_write_ppm(const char* filename, u8* data, u64 width, u64 height);
void                sta_draw_rect_to_image(u8* data, u64 image_width, u64 x, u64 y, u64 width, u64 height, u8 r, u8 g, u8 b, u8 a);
bool                sta_targa_read_from_file(TargaImage* image, const char* filename);
bool                sta_targa_read_from_file(Arena* arena, TargaImage* image, const char* filename);
bool                sta_read_file(Arena* arena, Buffer* string, const char* fileName);
bool                sta_read_file(Buffer* buffer, const char* fileName);
bool                sta_append_to_file(const char* filename, const char* message);

#endif
