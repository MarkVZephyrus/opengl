#ifndef SHADER_H
#define SHADER_H

#include "glad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define false 0
#define true 1

typedef struct Shader {
  int ID; // Program ID.
} Shader;
Shader shaderDef(const char *vertexPath, const char *fragmentPath);
void useShader(Shader shader);
void setBool(Shader shader, const char *name, bool value);
void setInt(Shader shader, const char *name, int value);
void setFloat(Shader shader, const char *name, float value);

#endif // !SHADER_H
#define SHADER_H
