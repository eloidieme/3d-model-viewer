# Deltaviewer

A modern, high-performance 3D model viewer written in C++17 using OpenGL 3.3+.

Deltaviewer is designed with a clean architecture, supporting hot-reloadable shaders, runtime configuration via JSON, and efficient mesh rendering. It leverages **Assimp** for broad model format support, **GLFW** for windowing, **GLM** for mathematics, and **Dear ImGui** for the user interface.

## Features

- **Model Support:** Loads varied 3D formats (OBJ, FBX, GLTF, etc.) via the Open Asset Import Library (Assimp).
- **Interactive UI:** Real-time control over render settings, lighting, and camera statistics via a built-in ImGui dashboard.
- **Advanced Clipping System:** - Add and manipulate up to 8 arbitrary clipping planes in real-time.
  - Interactive controls for plane normals and distance.
  - **Visual Debugging:** Semi-transparent plane visualizers in the 3D scene to assist with positioning.
- **Shader Hot-Reloading:** Edit `.glsl` shader files and press `R` to reload them instantly without restarting the app.
- **Runtime Configuration:** Modify window resolution, camera sensitivity, and render settings via `config.json` without recompiling.
- **Performance:** Optimized mesh loading with move semantics and cached transform matrices.
- **Camera System:** First-person flying camera with adjustable speed and FOV.
- **Cross-Platform:** Robust path handling using C++17 `<filesystem>` (Windows/macOS/Linux compatible).

## Prerequisites

- **CMake** (3.11 or higher)
- **C++ Compiler** with C++17 support (GCC, Clang, or MSVC)
- **Git** (required to fetch dependencies)

## Build Instructions

This project uses CMake's `FetchContent` to automatically download dependencies (GLFW, GLM, Assimp).

1.  **Clone the repository:**

    ```bash
    git clone [https://github.com/eloidieme/deltaviewer.git](https://github.com/eloidieme/deltaviewer.git)
    cd deltaviewer
    ```

2.  **Create a build directory:**

    ```bash
    mkdir build
    cd build
    ```

3.  **Configure and Build:**
    ```bash
    cmake ..
    cmake --build .
    ```

## Usage

### Running the Viewer

By default, the viewer loads the model specified in `config.json`. You can also override this by passing a path as a command-line argument:

```bash
# Run with default model (defined in config.json)
./main

# Run with a specific model
./main path/to/your/model.obj
```

> **Note:** Ensure `config.json` and the `assets/` folder are in the same directory as the executable (or in the project root if running from an IDE).

### Controls

| Key                   | Action                                          |
| :-------------------- | :---------------------------------------------- |
| **W / Z** or **Up**   | Move Forward                                    |
| **S** or **Down**     | Move Backward                                   |
| **A / Q** or **Left** | Move Left                                       |
| **D** or **Right**    | Move Right                                      |
| **Mouse**             | Look around                                     |
| **R**                 | **Hot-Reload Shaders** (Recompiles .glsl files) |
| **Esc**               | Exit Application                                |

_Note: WASD/ZQSD support is handled automatically to support QWERTY and AZERTY layouts._

### UI Overlay

The application features a "Settings" window that allows you to:

- View application stats (FPS).
- Change the clear color and light position.
- **Clipping Planes**: Dynamically add, remove, and adjust clipping planes to inspect the interior of your models.

## Configuration (`config.json`)

You can customize the application settings by editing `config.json` in the root directory. No recompilation is needed.

```json
{
  "Window": {
    "Width": 1280,
    "Height": 960,
    "Title": "Deltaviewer"
  },
  "Render": {
    "ClearColor": [0.1, 0.1, 0.2, 1.0],
    "LightPosition": [2.0, 2.0, 2.0],
    "ClippingPlane": [0.5, 0.5, 0.0, -0.5]
  },
  "Camera": {
    "MovementSpeed": 2.5,
    "Sensitivity": 0.1,
    "Fov": 45.0,
    "StartPosition": [0.0, 0.0, 7.5],
    "NearPlane": 0.1,
    "FarPlane": 100.0
  },
  "Paths": {
    "DefaultModel": "assets/models/backpack/backpack.obj",
    "ShaderVert": "assets/shaders/vert.glsl",
    "ShaderFrag": "assets/shaders/frag.glsl"
  }
}
```

## Project Structure

- **src/**: Source code.
  - **Core/**: Windowing, Input handling, Events, Transforms.
  - **Graphics/**: OpenGL wrappers (Renderer, Shader, Texture, Mesh).
  - **Scene/**: Model loading and node processing.
  - **App.cpp**: Main application loop, UI logic, and rendering pipeline.
  - **Config.cpp**: JSON parsing and global settings.
- **assets/**: Shaders (including new plane visualization shaders) and default models.
- **vendor/**: Third-party libraries (GLAD, stb_image, nlohmann_json, ImGui).
