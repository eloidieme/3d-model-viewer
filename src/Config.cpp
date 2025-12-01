#include "Config.hpp"

#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace glm {
void from_json(const json &j, glm::vec3 &v) {
  if (j.is_array() && j.size() >= 3) {
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];
  }
}

void from_json(const json &j, glm::vec4 &v) {
  if (j.is_array() && j.size() >= 4) {
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];
    v.w = j[3];
  }
}
} // namespace glm

namespace Config {
namespace Window {
unsigned int Width = 1280;
unsigned int Height = 960;
std::string Title = "Deltaviewer";
} // namespace Window

namespace Render {
glm::vec4 ClearColor = {0.1f, 0.1f, 0.2f, 1.0f};
glm::vec3 LightPosition = {2.0f, 2.0f, 2.0f};
} // namespace Render

namespace Camera {
float MovementSpeed = 2.5f;
float Sensitivity = 0.1f;
float Fov = 45.0f;
float NearPlane = 0.1f;
float FarPlane = 100.0f;
glm::vec3 StartPosition = {0.0f, 0.0f, 7.5f};
} // namespace Camera

namespace Paths {
std::string DefaultModel = "assets/models/backpack/backpack.obj";
std::string ShaderVert = "assets/shaders/vert.glsl";
std::string ShaderFrag = "assets/shaders/frag.glsl";
std::string PlaneShaderVert = "assets/shaders/plane_vert.glsl";
std::string PlaneShaderFrag = "assets/shaders/plane_frag.glsl";
} // namespace Paths

void load(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cout << "Config: Could not find " << path << ". Using defaults."
              << std::endl;
    return;
  }

  try {
    json j;
    file >> j;

    if (j.contains("Window")) {
      auto &w = j["Window"];
      if (w.contains("Width"))
        Window::Width = w["Width"];
      if (w.contains("Height"))
        Window::Height = w["Height"];
      if (w.contains("Title"))
        Window::Title = w["Title"];
    }

    if (j.contains("Render")) {
      auto &r = j["Render"];
      if (r.contains("ClearColor"))
        r["ClearColor"].get_to(Render::ClearColor);
      if (r.contains("LightPosition"))
        r["LightPosition"].get_to(Render::LightPosition);
    }

    if (j.contains("Camera")) {
      auto &c = j["Camera"];
      if (c.contains("MovementSpeed"))
        Camera::MovementSpeed = c["MovementSpeed"];
      if (c.contains("Sensitivity"))
        Camera::Sensitivity = c["Sensitivity"];
      if (c.contains("Fov"))
        Camera::Fov = c["Fov"];
      if (c.contains("StartPosition"))
        c["StartPosition"].get_to(Camera::StartPosition);
      if (c.contains("NearPlane"))
        Camera::NearPlane = c["NearPlane"];
      if (c.contains("FarPlane"))
        Camera::FarPlane = c["FarPlane"];
    }

    if (j.contains("Paths")) {
      auto &p = j["Paths"];
      if (p.contains("DefaultModel"))
        Paths::DefaultModel = p["DefaultModel"];
      if (p.contains("ShaderVert"))
        Paths::ShaderVert = p["ShaderVert"];
      if (p.contains("ShaderFrag"))
        Paths::ShaderFrag = p["ShaderFrag"];
    }

    std::cout << "Config: Loaded " << path << " successfully." << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Config: Error parsing JSON: " << e.what() << std::endl;
  }
}
} // namespace Config