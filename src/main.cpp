#define GLM_FORCE_RADIANS
#define STB_IMAGE_IMPLEMENTATION

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "include/stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "include/Callbacks.hpp"
#include "include/Floor.hpp"
#include "include/GameObject.hpp"
#include "include/Model.hpp"
#include "include/Shader.hpp"

using namespace glm;
using namespace std;

extern int drinksDrank;
extern bool canDrink;
extern bool isDrinking;

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
  glfwSetKeyCallback(window, Callbacks::key_callback);

  Shader shader("shaders/simple.vert", "shaders/simple.frag");

  shader.use();

  static mat4 P = perspective(radians(50.0f), 1.0f, 0.1f, 50.0f);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  static Model pedestal("models/pedestal/Pedestal.obj");
  static Model chandelier("models/chandelier/Chandelier.obj");
  static Floor floor;

  static vector<Model> bottleModels;
  bottleModels.push_back(Model("models/bottles/bottle1/Bottle1.obj"));
  bottleModels.push_back(Model("models/bottles/bottle2/Bottle2.obj"));
  bottleModels.push_back(Model("models/bottles/bottle3/Bottle3.obj"));

  static vector<GameObject> bottles, pedestals;
  // Create 8 bottle instances (one for each pedestal position)
  // and 8 pedestal instances (one for each position in the semi-circle)
  for (int i = 0; i < 8; ++i) {
    CollisionSphere bottleSphere;
    bottleSphere.radius = 1.0f;
    GameObject bottleObject(&bottleModels[i % bottleModels.size()],
                            bottleSphere);
    bottles.push_back(bottleObject);

    CollisionSphere pedestalSphere;
    pedestalSphere.radius = 0.5f;
    GameObject pedestalObject(&pedestal, pedestalSphere);
    pedestals.push_back(pedestalObject);
  }

  CollisionSphere cameraSphere;
  cameraSphere.radius = 0.5f;
  GameObject cameraCollider(nullptr, cameraSphere);

  while (!glfwWindowShouldClose(window)) {
    if (shader.shouldRecompile) {
      shader.recompile();
    }

    cameraSphere.center = vec3(cameraPos.x, 0.0f, cameraPos.z);
    cameraCollider.setCollisionSphere(cameraSphere);

    shader.setMat4("P", P);
    shader.setFloat("time", glfwGetTime());
    shader.setFloat("disortionFactor",
                    drinksDrank > 0 ? 0.003f * drinksDrank : 0.0f);

    // Ustawenie oświetlenia
    shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
    shader.setVec3("lightPos1", 0.0f, 7.5f, -8.5f);
    shader.setVec3("lightColor1", 1.0f, 1.0f, 1.0f);
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
    static float radius = 7.0f;
    static float centerZ = -5.0f;
    for (int i = 0; i < 8; ++i) {
      // Calculate angle for a semi-circle (from 180 degrees down to 0
      // degrees)
      float angle = radians(180.0f - (i * 180.0f / 7.0f));
      float x = radius * cos(angle);
      float z = centerZ - radius * sin(angle);

      mat4 M = mat4(1.0f);
      M = translate(M, vec3(x, 0.0f, z));
      M = rotate(M, angle - radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
      shader.setMat4("M", M);

      CollisionSphere pedestalSphere = pedestals[i].getCollisionSphere();
      pedestalSphere.center = vec3(x, 0.0f, z);
      pedestals[i].setCollisionSphere(pedestalSphere);

      pedestals[i].Draw(shader);
    }

    // 8 bottles on pedestals
    for (int i = 0; i < 8; ++i) {
      GameObject &bottleObject = bottles[i % bottles.size()];

      float angle = radians(180.0f - (i * 180.0f / 7.0f));
      float x = radius * cos(angle);
      float z = centerZ - radius * sin(angle);

      mat4 M = mat4(1.0f);
      M = translate(M, vec3(x, .0f, z));
      M = rotate(M, angle - radians(90.0f), vec3(0.0f, 1.0f, 0.0f));

      CollisionSphere bottleSphere = bottleObject.getCollisionSphere();
      bottleSphere.center = vec3(x, 0.0f, z);
      bottleObject.setCollisionSphere(bottleSphere);

      shader.setMat4("M", M);
      bottleObject.Draw(shader);
    }

    // Floor
    mat4 floorM = mat4(1.0f);
    floorM = translate(floorM, vec3(0.0f, -1.0f, 0.0f));
    shader.setMat4("M", floorM);
    floor.Draw(shader);

    for (const auto &pedestal : pedestals) {
      if (cameraCollider.checkCollision(pedestal)) {
        vec3 direction = normalize(pedestal.getCollisionSphere().center -
                                   cameraSphere.center);
        cameraPos -= direction * 0.1f;
      }
    }

    for (int i = 0; i < bottles.size(); ++i) {
      const auto &bottle = bottles[i];
      if (cameraCollider.checkCollision(bottle)) {
        cout << "[Collision] Detected collision with bottle " << i << endl;
        canDrink = true;
        break; // Sprawdzamy do pierwszego wykrycia
      } else {
        canDrink = false;
      }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window); // Delete OpenGL context and the window.
  glfwTerminate();           // Free GLFW resources
  exit(EXIT_SUCCESS);
}
