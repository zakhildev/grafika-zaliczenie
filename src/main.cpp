#include <algorithm>
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

#ifdef __linux__
  // Modern Linux desktop require to pass GLFW_PLATFORM_X11 hint to GLFW
  // in order to use OpenGL context with X11 instead of Wayland.
  const char *session_type = getenv("XDG_SESSION_TYPE");
  if (session_type && strcmp(session_type, "wayland") == 0 &&
      getenv("DISPLAY")) {
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
  }
#endif

  if (!glfwInit()) {
    fprintf(stderr, "Can't initialize GLFW.\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Disable window resizing to simplify things
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(720, 720, "Alcohol Gallery", NULL, NULL);

  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
    exit(EXIT_FAILURE);
  }

  initOpenGLProgram(window);
  glfwSetCursorPosCallback(window, Callbacks::mouse_callback);

  Shader shader("shaders/simple.vert", "shaders/simple.frag");

  shader.use();

  static mat4 P = perspective(radians(50.0f), 1.0f, 0.1f, 50.0f);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  static Model pedestal("models/pedestal/Pedestal.obj");
  static Model chandelier("models/chandelier/Chandelier.obj");
  static Floor floor;

  static vector<Model> bottles;
  bottles.push_back(Model("models/bottles/bottle1/Bottle1.obj"));
  bottles.push_back(Model("models/bottles/bottle2/Bottle2.obj"));
  bottles.push_back(Model("models/bottles/bottle3/Bottle3.obj"));

  while (!glfwWindowShouldClose(window)) {
    if (shader.shouldRecompile) {
      shader.recompile();
    }

    shader.setMat4("P", P);

    // Set ambient color uniform
    shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);

    // Set lighting uniforms - Light 1
    shader.setVec3("lightPos1", 0.0f, 7.5f, -8.5f);
    shader.setVec3("lightColor1", 1.0f, 1.0f, 1.0f);

    // Set lighting uniforms - Light 2
    shader.setVec3("lightPos2", 0.0f, 7.5f, 8.5f);
    shader.setVec3("lightColor2", 1.0f, 1.0f, 1.0f);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    Callbacks::processInput(window, deltaTime);

    shader.use();
    mat4 V = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader.setMat4("V", V);
    shader.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

    glClearColor(0.81f, 0.92f, 0.97f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Chandelier 1
    mat4 chandelierM = mat4(1.0f);
    chandelierM = translate(chandelierM, vec3(0.0f, 7.5f, -8.5f));
    chandelierM = scale(chandelierM, vec3(2.0f, 2.0f, 2.0f));
    shader.setMat4("M", chandelierM);
    chandelier.Draw(shader);

    // Chandelier 2
    chandelierM = mat4(1.0f);
    chandelierM = translate(chandelierM, vec3(0.0f, 7.5f, 8.5f));
    chandelierM = scale(chandelierM, vec3(2.0f, 2.0f, 2.0f));
    shader.setMat4("M", chandelierM);
    chandelier.Draw(shader);

    // 8 Pedestals in a semi-circle
    float radius = 7.0f;
    float centerZ = -5.0f;
    for (int i = 0; i < 8; ++i) {
      // Calculate angle for a semi-circle (from 180 degrees down to 0 degrees)
      float angle = radians(180.0f - (i * 180.0f / 7.0f));
      float x = radius * cos(angle);
      float z = centerZ - radius * sin(angle);

      mat4 M = mat4(1.0f);
      M = translate(M, vec3(x, 0.0f, z));
      // Rotate pedestals to face the center of the room
      M = rotate(M, angle - radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
      shader.setMat4("M", M);
      pedestal.Draw(shader);
    }

    // 8 bottles on pedestals
    for (int i = 0; i < 8; ++i) {
      float angle = radians(180.0f - (i * 180.0f / 7.0f));
      float x = radius * cos(angle);
      float z = centerZ - radius * sin(angle);

      Model bottle = bottles[i % bottles.size()];

      mat4 M = mat4(1.0f);
      M = translate(M, vec3(x, .0f, z));
      M = rotate(M, angle - radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
      shader.setMat4("M", M);
      bottle.Draw(shader);
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
