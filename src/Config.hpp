#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Config {
void load(const std::string &path);

namespace Window {
extern unsigned int Width;
extern unsigned int Height;
extern std::string Title;
} // namespace Window

namespace Render {
extern glm::vec4 ClearColor;
extern glm::vec3 LightPosition;
extern glm::vec4 ClippingPlane;
} // namespace Render

namespace Camera {
extern float MovementSpeed;
extern float Sensitivity;
extern float Fov;
extern float NearPlane;
extern float FarPlane;
extern glm::vec3 StartPosition;
} // namespace Camera

namespace Paths {
extern std::string DefaultModel;
extern std::string ShaderVert;
extern std::string ShaderFrag;
} // namespace Paths
} // namespace Config