#include "headers/glad.h"
#include "headers/shader.h"
#include "headers/stb_image.h" // Shame on you, you little bitch
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 600

// Of Course the simple things get fucked up
// Under current directory structre you can either have these paths relative to
// the lib file where the go or use realpath()
// SOLUTION: Changed the file structre.
#define VERTEX_SOURCE_PATH "shaders/vertex.vert"
#define FRAGMENT_SOURCE_PATH "shaders/fragment.frag"
#define TEXTURE_SOURCE_PATH "objects/textures/grunge-wall.jpg"

void framebuffer_size_callback(GLFWwindow *, int, int);
void processInput(GLFWwindow *);

int main() {
  // Window options.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "OpenGL", NULL, NULL);
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
  glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float rectVert[] = {
      // Vertex           // Colors         // Texture CoOrds
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // top left
  };
  unsigned int rectIndices[] = {
      0, 1, 3, // triag 1
      1, 2, 3  // triag 2
  };

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
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectVert), rectVert, GL_STATIC_DRAW);

  // EBO
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices,
               GL_STATIC_DRAW);

  // SHADER
  Shader shader = shaderDef(VERTEX_SOURCE_PATH, FRAGMENT_SOURCE_PATH);

  // TEXTURE
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // texture options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // (s, t, r) = (x, y, z)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and gen the texture
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load(TEXTURE_SOURCE_PATH, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

  } else
    puts("ERR::TEXTURE_FAILED_TO_BE_LOADED");
  stbi_image_free(data);

  // Vertex Attributes
  // bascically tells opengl how the vertices should be interpretted by OpenGL.
  // 3 * float = vertex | 3 * float = color | 2 * float = 2d texture
  // Total 8 * float for each vertex.
  // Vertex Coorinate Data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // These automatically go to VAO
  // Color Attribute Array
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // Texture Attribute Array
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

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

    glBindTexture(GL_TEXTURE_2D, texture);

    useShader(shader); // Use the shaders.
    setFloat(shader, "offset", timeValue);

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

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

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
