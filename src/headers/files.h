#ifndef RESOURCES_H
#define RESOURCES_H

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

#define PATH_SIZE 100

enum ShaderTypes { fragment, vertex };
typedef struct ShaderRe {
  char *path;
  int len;
  ShaderTypes type;
  FILE *file;
  char *code;
} ShaderFile;

ShaderFile *shaderFile(char *path, ShaderTypes type);
FILE *getShaderFilePointer(ShaderFile shaderFile);
char *getShaderCode(ShaderFile shaderFile);

enum Shapes { triangle, rectangle, polygon };
typedef struct ShapeRes {
  char *path;
  int len;
  char *type;
  float *vertices;
  int vertCount;
} ShapeFile;

#endif // !RESOURSCES_H
#define RESOURCES_H
