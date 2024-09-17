#include "shader.h"
#include <stdio.h>

char *vertexCode, *fragmentCode;
FILE *vShaderFile, *fShaderFile;

Shader shaderDef(const char *vertexPath, const char *fragmentPath) {
  Shader shader;

  vShaderFile = fopen(vertexPath, "r");
  fShaderFile = fopen(fragmentPath, "r");

  if (vShaderFile == NULL || fShaderFile == NULL) {
    printf("ERROR::SHADER::FAILED_TO_OPEN_FILE(S)");
    exit(1);
  }

  fscanf(vShaderFile, "%c", vertexCode);
  fscanf(fShaderFile, "%c", fragmentCode);

  fclose(vShaderFile);
  fclose(fShaderFile);

  const char *vShaderCode = vertexCode;
  const char *fShaderCode = fragmentCode;

  // compilation

  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  printf(vertexCode, "\n", fragmentCode, "\n");
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    printf("VERTEX SHADER COMPILE ERROR\n%s\n", infoLog);
  }

  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    printf("FRAGMENT SHADER COMPILE ERROR\n%s\n", infoLog);
  }

  // Linking
  shader.ID = glCreateProgram();
  glAttachShader(shader.ID, vertex);
  glAttachShader(shader.ID, fragment);
  glLinkProgram(shader.ID);

  glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader.ID, 512, NULL, infoLog);
    printf("SHADER LINKING ERROR\n%s\n", infoLog);
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return shader;
}

void useShader(Shader shader) { glUseProgram(shader.ID); }

void setBool(Shader shader, const char *name, int value) {
  glUniform1i(glGetUniformLocation(shader.ID, name), value);
}

void setInt(Shader shader, const char *name, int value) {
  glUniform1i(glGetUniformLocation(shader.ID, name), value);
}

void setFloat(Shader shader, const char *name, float value) {
  glUniform1f(glGetUniformLocation(shader.ID, name), value);
}
