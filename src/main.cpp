#include "App.hpp"
#include "Config.hpp"
#include "Core/Log.hpp"

#include <filesystem>

int main(int argc, char *argv[]) {
  Log::init();
  LOG_CORE_WARN("Initialized Log System!");

  Config cfg = Config::load("config.json");

  std::filesystem::path modelPath = cfg.paths.DefaultModel;
  if (argc > 1) {
    modelPath = argv[1];
  }

  if (!std::filesystem::exists(modelPath)) {
    LOG_CORE_CRITICAL("FATAL: Path does not exist: {0}", modelPath.string());
    return EXIT_FAILURE;
  }

  try {
    App app(cfg, modelPath);
    app.run();
  } catch (const std::exception &e) {
    LOG_CORE_CRITICAL("Application Crash: {0}", e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}