#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

extern vec3 cameraPos;
extern vec3 cameraFront;
extern vec3 cameraUp;

extern int drinksDrank;
extern bool canDrink;   // True, jeśli w zasięgu butelki
extern bool isDrinking; // True podczas animacji picia

extern float drinkingAnimationTime;
extern int drinkingBottleIndex;
extern const float drinkingDuration;

namespace Callbacks {

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);
void processInput(GLFWwindow *window, float deltaTime);

} // namespace Callbacks