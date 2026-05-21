#include "../include/GameObject.hpp"

using namespace std;

GameObject::GameObject(Model *model, const CollisionSphere &collisionSphere)
    : model(model), collisionSphere(collisionSphere) {}

CollisionSphere GameObject::getCollisionSphere() const {
  return collisionSphere;
}

void GameObject::setCollisionSphere(const CollisionSphere &sphere) {
  collisionSphere = sphere;
}

void GameObject::Draw(Shader &shader) {
  if (model != nullptr) {
    model->Draw(shader);
  }
}