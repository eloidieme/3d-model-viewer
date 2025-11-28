#include "App.hpp"

#include <filesystem>

int main(int argc, char *argv[]) {
  std::filesystem::path modelPath = "assets/models/backpack/backpack.obj";

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