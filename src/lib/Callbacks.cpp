#include "../include/Callbacks.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

using namespace glm;
using namespace std;

vec3 cameraPos = vec3(0.0f, 2.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

int drinksDrank = 0;
bool canDrink = false;
bool isDrinking = false;
float drinkingAnimationTime = 0.0f;
const float drinkingDuration = 0.5f;

namespace Callbacks {

static bool firstMouse = true;
static float yaw = -90.0f;
static float pitch = 0.0f;
static float lastX = 720.0f / 2.0;
static float lastY = 720.0f / 2.0;

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  vec3 front;
  front.x = cos(radians(yaw)) * cos(radians(pitch));
  front.y = sin(radians(pitch));
  front.z = sin(radians(yaw)) * cos(radians(pitch));
  cameraFront = normalize(front);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_E && action == GLFW_PRESS) {
    if (canDrink && !isDrinking) {
      isDrinking = true;
      drinkingAnimationTime = 0.0f;
      drinksDrank++;
      cout << "You drank! Total drinks: " << drinksDrank << endl;
    } else if (isDrinking) {
      cout << "Already drinking!" << endl;
    } else {
      cout << "No bottle nearby to drink from!" << endl;
    }
  }
}

void processInput(GLFWwindow *window, float deltaTime) {
  float cameraSpeed = 5.0f * deltaTime;
  vec3 horizontalFront = normalize(vec3(cameraFront.x, 0.0f, cameraFront.z));

  // Zaburzenie chodzenia liczone ze zmiennej drinksDrank
  float walkingDisortion = sin(drinksDrank * 0.5f) * 0.15f;
  vec3 disturbance = vec3(walkingDisortion * sin(glfwGetTime() * 2.0f), 0,
                          walkingDisortion * cos(glfwGetTime() * 2.0f));

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * horizontalFront + disturbance * 0.2f;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * horizontalFront + disturbance * 0.2f;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= normalize(cross(horizontalFront, cameraUp)) * cameraSpeed +
                 disturbance * 0.1f;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += normalize(cross(horizontalFront, cameraUp)) * cameraSpeed +
                 disturbance * 0.1f;

  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
  }

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

} // namespace Callbacks
