#pragma once

#include "Core/InputActions.hpp"
#include "Core/KeyCodes.hpp"
#include <glm/glm.hpp>
#include <map>
#include <string>

struct WindowConfig {
  unsigned int Width = 1280;
  unsigned int Height = 960;
  std::string Title = "Deltaviewer";
};

struct RenderConfig {
  glm::vec4 ClearColor = {0.1f, 0.1f, 0.2f, 1.0f};
  glm::vec3 LightPosition = {2.0f, 2.0f, 2.0f};
};

struct CameraConfig {
  float MovementSpeed = 2.5f;
  float Sensitivity = 0.1f;
  float Fov = 45.0f;
  float NearPlane = 0.1f;
  float FarPlane = 100.0f;
  glm::vec3 StartPosition = {0.0f, 0.0f, 7.5f};
};

struct PathConfig {
  std::string DefaultModel = "assets/models/backpack/backpack.obj";
  std::string ShaderVert = "assets/shaders/vert.glsl";
  std::string ShaderFrag = "assets/shaders/frag.glsl";
  std::string PlaneShaderVert = "assets/shaders/plane_vert.glsl";
  std::string PlaneShaderFrag = "assets/shaders/plane_frag.glsl";
};

struct Config {
  WindowConfig window;
  RenderConfig render;
  CameraConfig camera;
  PathConfig paths;

  std::map<Action, KeyCode> bindings;

  static Config load(const std::string &path);
};