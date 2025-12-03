#include "Config.hpp"
#include "Core/Log.hpp"

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

Action stringToAction(const std::string &str) {
  if (str == "MoveForward")
    return Action::MoveForward;
  if (str == "MoveBackward")
    return Action::MoveBackward;
  if (str == "MoveLeft")
    return Action::MoveLeft;
  if (str == "MoveRight")
    return Action::MoveRight;
  if (str == "MoveUp")
    return Action::MoveUp;
  if (str == "ToggleCursor")
    return Action::ToggleCursor;
  if (str == "ReloadShader")
    return Action::ReloadShader;
  return Action::None;
}

Config Config::load(const std::string &path) {
  Config config;

  config.bindings[Action::MoveForward] = KeyCode::W;
  config.bindings[Action::MoveBackward] = KeyCode::S;
  config.bindings[Action::MoveLeft] = KeyCode::A;
  config.bindings[Action::MoveRight] = KeyCode::D;
  config.bindings[Action::MoveUp] = KeyCode::Space;
  config.bindings[Action::MoveDown] = KeyCode::LeftShift;
  config.bindings[Action::ToggleCursor] = KeyCode::Tab;
  config.bindings[Action::ReloadShader] = KeyCode::R;

  std::ifstream file(path);
  if (!file.is_open()) {
    LOG_CORE_WARN("Config: Could not find {0}. Using defaults.", path);
    return config;
  }

  try {
    json j;
    file >> j;

    if (j.contains("Window")) {
      auto &w = j["Window"];
      if (w.contains("Width"))
        config.window.Width = w["Width"];
      if (w.contains("Height"))
        config.window.Height = w["Height"];
      if (w.contains("Title"))
        config.window.Title = w["Title"];
    }

    if (j.contains("Render")) {
      auto &r = j["Render"];
      if (r.contains("ClearColor"))
        r["ClearColor"].get_to(config.render.ClearColor);
      if (r.contains("LightPosition"))
        r["LightPosition"].get_to(config.render.LightPosition);
    }

    if (j.contains("Camera")) {
      auto &c = j["Camera"];
      if (c.contains("MovementSpeed"))
        config.camera.MovementSpeed = c["MovementSpeed"];
      if (c.contains("Sensitivity"))
        config.camera.Sensitivity = c["Sensitivity"];
      if (c.contains("Fov"))
        config.camera.Fov = c["Fov"];
      if (c.contains("StartPosition"))
        c["StartPosition"].get_to(config.camera.StartPosition);
      if (c.contains("NearPlane"))
        config.camera.NearPlane = c["NearPlane"];
      if (c.contains("FarPlane"))
        config.camera.FarPlane = c["FarPlane"];
    }

    if (j.contains("Paths")) {
      auto &p = j["Paths"];
      if (p.contains("DefaultModel"))
        config.paths.DefaultModel = p["DefaultModel"];
      if (p.contains("ShaderVert"))
        config.paths.ShaderVert = p["ShaderVert"];
      if (p.contains("ShaderFrag"))
        config.paths.ShaderFrag = p["ShaderFrag"];
    }

    if (j.contains("Bindings")) {
      for (auto &[key, value] : j["Bindings"].items()) {
        Action action = stringToAction(key);
        if (action != Action::None) {
          config.bindings[action] = (KeyCode)value.get<int>();
        }
      }
    }

    LOG_CORE_INFO("Config: Loaded {0} successfully.", path);

  } catch (const std::exception &e) {
    LOG_CORE_ERROR("Config: Error parsing JSON: {0}", e.what());
  }

  return config;
}