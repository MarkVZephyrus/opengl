#include "headers/glad.h"
#include "headers/shader.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Of Course the simple things get fucked up
// Under current directory strctor you can either have these paths relative to
// the lib file where the go or use realpath()
#define VERTEX_SOURCE_PATH "shaders/vertex.vert"
#define FRAGMENT_SOURCE_PATH "shaders/fragment.frag"

float triangleVert[] = {-0.5f, -0.5f, 0.0f,  0.0f, 0.5f,
                        0.0f,  0.5f,  -0.5f, 0.0f};
unsigned int triagIndices[] = {0, 1, 2};

float rectVert[] = {
    0.5f,  0.5f,  0.0f, // top right
    0.5f,  -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f  // top left
};
unsigned int rectIndices[] = {
    0, 1, 3, // triag 1
    1, 2, 3  // triag 2
};

void framebuffer_size_callback(GLFWwindow *, int, int);
void processInput(GLFWwindow *);
unsigned int shaderCompile(const char *shaderSource, int TYPE);
void shaderLog(unsigned int shaderObject, int TYPE);

int main() {
  // Window options.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window.\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD.\n");
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Vertex Array Object
  // From what I understand, this object stores Vertex Attribute configurations
  // and uses them to access data from VBO.
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // VBO
  // Has all the vertex data
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind to the VBO
  /* glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVert), triangleVert, */
  /*              GL_STATIC_DRAW); // Give it data. */
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVert), triangleVert,
               GL_STATIC_DRAW);

  // EBO
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triagIndices), triagIndices,
               GL_STATIC_DRAW);

  Shader shader = shaderDef(VERTEX_SOURCE_PATH, FRAGMENT_SOURCE_PATH);

  // Vertex Attributes
  // bascically tells opengl how the vertices should be interpretted by OpenGL.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // These automatically go to VAO

  // From what I understand you don't need to attach to the VBOs and VAOs
  // explicitly. OpenGL does that for you automatically in the background, once
  // you attach an object to a specific type.

  // Render loop.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
    useShader(shader); // Use the shaders.
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    glBindVertexArray(VAO);
    // So ideally, everytime you draw you would bind to the VAO you need and
    // then unbind after drawing. So here is the redundant code for that case.
    // Redundant because in this case there is only one object.
    // But rebinding it every iteration bascically refreshes it.

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}

unsigned int shaderCompile(const char *shaderSource, int TYPE) {
  unsigned int shader;
  shader = glCreateShader(TYPE);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader); // Compilation
  shaderLog(shader, GL_COMPILE_STATUS);
  return shader;
}

void shaderLog(unsigned int shader, int TYPE) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, TYPE, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("SHADER ERROR\n%s\n", infoLog);
  }
}
