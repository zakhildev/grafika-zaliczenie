#define GLM_FORCE_RADIANS
#define STB_IMAGE_IMPLEMENTATION

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "include/stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "include/Callbacks.hpp"
#include "include/Floor.hpp"
#include "include/Model.hpp"
#include "include/Shader.hpp"

using namespace glm;
using namespace std;

int drinksDrank = 0;

void initOpenGLProgram(GLFWwindow *window) {
  stbi_set_flip_vertically_on_load(true);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(void) {
  GLFWwindow *window;

  if (!glfwInit()) {
    fprintf(stderr, "Can't initialize GLFW.\n");
    exit(EXIT_FAILURE);
  }

  // MacOS requires to specify OpenGL version and profile
#ifdef __APPLE__
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  // Disable window resizing to simplify things
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(720, 720, "Alcohol Gallery", NULL, NULL);

  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  GLenum err;
  if ((err = glewInit()) != GLEW_OK) {
    fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
    exit(EXIT_FAILURE);
  }

  initOpenGLProgram(window);
  glfwSetCursorPosCallback(window, Callbacks::mouse_callback);

  Shader shader("shaders/simple.vert", "shaders/simple.frag");

  shader.use();

  static mat4 P = perspective(radians(50.0f), 1.0f, 0.1f, 50.0f);

  shader.setMat4("P", P);

  // Set lighting uniforms
  shader.setVec3("lightPos", 0.0f, 20.0f, 0.0f);
  shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  static Model pedestal("models/pedestal/Pedestal.obj");
  static Floor floor;

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    Callbacks::processInput(window, deltaTime);

    shader.use();
    mat4 V = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader.setMat4("V", V);
    shader.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 8 Pedestals in a semi-circle
    float radius = 7.0f;
    float centerZ = -5.0f;
    for (int i = 0; i < 8; ++i) {
      // Calculate angle for a semi-circle (from 180 degrees down to 0 degrees)
      // We use 7.0f to divide the 180 degrees into 7 intervals for 8 pedestals
      float angle = radians(180.0f - (i * 180.0f / 7.0f));
      float x = radius * cos(angle);
      float z = centerZ - radius * sin(angle);

      mat4 M = mat4(1.0f);
      M = translate(M, vec3(x, 0.0f, z));
      shader.setMat4("M", M);
      pedestal.Draw(shader);
    }

    // Floor
    mat4 floorM = mat4(1.0f);
    floorM = translate(floorM, vec3(0.0f, -1.0f, 0.0f));
    shader.setMat4("M", floorM);
    floor.Draw(shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window); // Delete OpenGL context and the window.
  glfwTerminate();           // Free GLFW resources
  exit(EXIT_SUCCESS);
}
