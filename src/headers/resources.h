#ifndef RESOURCES_H
#define RESOURCES_H

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

#define PATH_SIZE 100

enum ShaderTypes { fragment, vertex };
typedef struct ShaderRes {
  char *path;
  int len;
  ShaderTypes type;
  FILE *file;
  char *code;
} ShaderRes;

ShaderRes *shaderRes(char *path, ShaderTypes type);
FILE *getShaderFilePointer(ShaderRes shaderFile);
char *getShaderCode(ShaderRes shaderFile);

enum Shapes { triangle, rectangle, polygon };
typedef struct ShapeRes {
  char *path;
  int len;
  char *type;
  float *vertices;
  float *indices;
  int vertCount;
} ShapeRes;

ShapeRes *shapeRes(const char *path, Shapes shapeType);
float *getShapeArray(ShapeRes shape);

#endif // !RESOURCES_H
#define RESOURCES_H
