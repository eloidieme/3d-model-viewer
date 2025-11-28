#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
  glm::vec3 Position;
  glm::vec3 Rotation;
  glm::vec3 Scale;

  glm::mat4 getModelMatrix() {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), Position);
    model = glm::rotate(model, glm::radians(Rotation.x),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(Rotation.y),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(Rotation.z),
                        glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, Scale);

    return model;
  }
};
