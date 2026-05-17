#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

namespace Callbacks {

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void processInput(GLFWwindow *window, float deltaTime);

}