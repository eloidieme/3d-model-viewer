#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Config {
namespace Window {
constexpr unsigned int Width = 1280;
constexpr unsigned int Height = 960;
const std::string Title = "Deltaviewer";
} // namespace Window

namespace Render {
const glm::vec4 ClearColor = {0.1f, 0.1f, 0.2f, 1.0f};

const glm::vec3 LightPosition = {2.0f, 2.0f, 2.0f};
const glm::vec4 ClippingPlane = {0.5f, 0.5f, 0.0f, -0.5f};
} // namespace Render

namespace Camera {
constexpr float MovementSpeed = 2.5f;
constexpr float Sensitivity = 0.1f;
constexpr float Fov = 45.0f;
constexpr float NearPlane = 0.1f;
constexpr float FarPlane = 100.0f;

const glm::vec3 StartPosition = {0.0f, 0.0f, 7.5f};
} // namespace Camera

namespace Paths {
const std::string DefaultModel = "assets/models/backpack/backpack.obj";
const std::string ShaderVert = "assets/shaders/vert.glsl";
const std::string ShaderFrag = "assets/shaders/frag.glsl";
} // namespace Paths
} // namespace Config