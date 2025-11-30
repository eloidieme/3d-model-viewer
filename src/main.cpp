#include "App.hpp"

#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
  Config::load("config.json");

  std::filesystem::path modelPath = Config::Paths::DefaultModel;

  if (argc > 1) {
    modelPath = argv[1];
  }

  if (!std::filesystem::exists(modelPath)) {
    std::cerr << "FATAL: Path does not exist: " << modelPath << std::endl;
    return EXIT_FAILURE;
  }

  try {
    App app(modelPath);
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}