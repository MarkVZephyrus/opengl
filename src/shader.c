#include "headers/shader.h"
#include <stdio.h>
#include <stdlib.h>
#define FILESIZE 1000
char *vertexCode;
char *fragmentCode;
FILE *vShaderFile, *fShaderFile;

Shader shaderDef(const char *vertexPath, const char *fragmentPath) {
  Shader shader;

  puts(vertexPath);
  vShaderFile = fopen(vertexPath, "rb");
  fShaderFile = fopen(fragmentPath, "rb");

  if (vShaderFile == NULL || fShaderFile == NULL) {
    puts("ERROR::SHADER::FAILED_TO_OPEN_FILE(S)");
    exit(1);
  }

  // File Read bloc
  long file_length = 0;
  fseek(vShaderFile, 0L, SEEK_END);
  file_length = ftell(vShaderFile);
  rewind(vShaderFile);
  vertexCode = malloc(file_length * sizeof(char) + 1);
  fread(vertexCode, sizeof(char), file_length, vShaderFile);
  vertexCode[file_length] = '\0';

  fseek(fShaderFile, 0L, SEEK_END);
  file_length = ftell(fShaderFile);
  rewind(fShaderFile);
  fragmentCode = malloc(file_length * sizeof(char) + 1);
  fread(fragmentCode, sizeof(char), file_length, fShaderFile);
  fragmentCode[file_length] = '\0';

  fclose(vShaderFile);
  fclose(fShaderFile);

  const char *vShaderCode = vertexCode;
  const char *fShaderCode = fragmentCode;

  /* free(vertexCode); */
  /* free(fragmentCode); */

  // compilation

  int success;
  char infoLog[512];

  // vertex Shader
  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    printf("VERTEX SHADER COMPILE ERROR\n%s\n", infoLog);
  }

  // fragment Shader
  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
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
