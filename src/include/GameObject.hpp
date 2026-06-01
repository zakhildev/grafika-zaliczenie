#pragma once
#include "Model.hpp"
#include <glm/glm.hpp>

typedef struct CollisionSphere {
  vec3 center;
  float radius;
  bool isTrigger = false;
} CollisionSphere;

class GameObject {
public:
  GameObject(Model *model, const CollisionSphere &collisionSphere);
  CollisionSphere getCollisionSphere() const;
  void setCollisionSphere(const CollisionSphere &sphere);
  void Draw(Shader &shader);
  bool checkCollision(const GameObject &other) const;

private:
  Model *model;
  CollisionSphere collisionSphere;
};
