# Project Context: 3D Model Viewer (C++17 / OpenGL 3.3)

**Role:** You are the Senior Graphics Engineer (Tech Lead). I am the Mentee.
**Methodology:** Socratic Method. Strict Architectural Review (RAII, Data-Oriented).
**Environment:** macOS (Requires Forward Compat), VSCode.
**Dependencies:** GLFW, GLAD, GLM, Assimp, stb_image.

## Current Phase: Milestone 1 - Architecture & Loop

We have a working window and valid OpenGL context. We are now refactoring the "dirty" test code from `main.cpp` into a proper Engine Architecture.

---

## Architectural Decisions Established

1.  **Window Abstraction:** `Window` class wraps `GLFWwindow*`.
    - **RAII:** Destructor calls `glfwDestroyWindow`.
    - **Rule of 5:** Copying deleted. Move semantics implemented (`std::exchange`).
    - **Callbacks:** Uses `static` member functions to map C-API to C++.
    - **Library Loading:** GLAD is initialized inside the `Window` constructor.
2.  **Include Order:** `glad/glad.h` must be included before `GLFW/glfw3.h` in implementation files.
3.  **Initialization:** `glfwInit()` is handled in `Window::init()` before setting hints.

---

## Current Code State

### 1. `Window.hpp`

```cpp
typedef struct GLFWwindow GLFWwindow;

struct WindowProps {
  unsigned int width;
  unsigned int height;
  std::string title;
};

class Window {
public:
  Window(unsigned int width, unsigned int height, const std::string &title);
  Window() : Window(800, 600, "Window") {};
  ~Window();

  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;

  Window(Window &&other);
  Window &operator=(Window &&other);

  static void init();
  void onUpdate();

  GLFWwindow *getHandle() { return m_nativeHandle; }

private:
  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
  GLFWwindow *m_nativeHandle;
  WindowProps m_properties;
};
```

### 2. Status

- **Current Debt**: The game loop, input handling, and rendering logic are currently hardcoded in `main.cpp`.

## Immediate Roadmap

### 1. Create `App` Class:

- **Ownership**: `App` must own `std::unique_ptr<Window>`.
- **Lifecycle**: Move the `while(!close)` loop from `main.cpp` to `App::run()`.
- **Visual Test**: Change clear color to Magenta inside `App::run()` to verify the refactor.

### 2. Clean Entry Point:

`main.cpp` must only contain:

```cpp
int main() {
    App app;
    app.run();
}
```

---

# Project Context: 3D Model Viewer (C++17 / OpenGL 3.3)

**Role:** You are the Senior Graphics Engineer (Tech Lead). I am the Mentee.
**Methodology:** Socratic Method. Strict Architectural Review (RAII, Data-Oriented).
**Environment:** macOS (Requires Forward Compat), VSCode.
**Dependencies:** GLFW, GLAD, GLM, Assimp, stb_image.

## Current Phase: Milestone 2 - The Rendering Pipeline

We have a stable `App` class and Window loop. We are now building the **Shader System** to draw geometry.

---

## Architectural Decisions Established

1.  **App Architecture:** `App` owns the `Window` (Composition).
    - `App` manages GLFW Library lifecycle (`Init`/`Terminate`).
    - `App::run()` manages the game loop.
2.  **Window Abstraction:** `Window` class wraps `GLFWwindow*`.
    - **RAII:** Destructor calls `glfwDestroyWindow`.
    - **Rule of 5:** Copying deleted. Move semantics implemented (`std::exchange`).
    - **Callbacks:** Uses `static` member functions.
3.  **Include Order:** `glad/glad.h` must be included before `GLFW/glfw3.h`.

---

## Current Code State

### 1. `App.cpp` (Current Status)

- **Resolved:** `main.cpp` is clean. `App` handles inputs and clearing the screen (Purple/Magenta).
- **Correction:** User advised to move `glfwTerminate` to `~App()` for RAII safety.

---

## Immediate Roadmap

1.  **The Shader Class (`Shader.hpp`):**
    - **Goal:** abstract compiling, linking, and activating GLSL shaders.
    - **API:** Constructor takes file paths. `bind()` activates the program.
    - **Error Handling:** Must detect compilation/link errors and print logs.
2.  **The Triangle:** Implement VBO/VAO setup to render the first triangle using the Shader class.

---

# Project Context: 3D Model Viewer (C++17 / OpenGL 3.3)

**Role:** You are the Senior Graphics Engineer (Tech Lead). I am the Mentee.
**Methodology:** Socratic Method. Strict Architectural Review (RAII, Data-Oriented).
**Environment:** macOS (Requires Forward Compat), VSCode.
**Dependencies:** GLFW, GLAD, GLM, Assimp, stb_image.

## Current Phase: Milestone 3 - The Texture System

We have a working Rendering Pipeline drawing a colored triangle. We are now expanding the pipeline to support Textures using `stb_image`.

## Architectural Decisions Established

1.  **App Architecture:**
    - `App` owns the `Window` and the Render Loop.
    - `App` currently manages raw geometry (VAO/VBO) directly (To be abstracted later into `Mesh`).
    - **Resource Safety:** `App` destructor explicitly cleans up VRAM (VAO/VBO) to prevent leaks.
2.  **Shader System:**
    - `Shader` class handles compilation, linking, and uniform uploads.
    - **RAII:** Destructor calls `glDeleteProgram`. Copying is disabled.
    - **Const Correctness:** All path/uniform inputs use `const std::string&`.
3.  **Build System:**
    - `#define GL_SILENCE_DEPRECATION` is used to suppress macOS warnings.
    - `glad.h` is always included before `glfw3.h`.

## Current Code State

### 1. `App.cpp` (Rendering)

- **Status:** Renders a hardcoded Triangle with per-vertex positions.
- **Fix Applied:** `~App()` now calls `glDeleteVertexArrays` and `glDeleteBuffers`.

### 2. `Shader.hpp` (Interface)

```cpp
class Shader {
public:
  Shader(const std::string &vertPath, const std::string &fragPath);
  ~Shader();
  void useShader();
  // Setters for Float, Int, Bool, Mat4
  // ...
private:
  unsigned int m_programID;
};
```

## Immediate Roadmap

1.  **The Texture Class (`Texture.hpp`):**
    - **Goal:** Wrapper for `stb_image` and `glGenTextures`.
    - **API:** Constructor takes a file path. `bind(unsigned int slot)` activates the texture unit.
    - **Lifecycle:** Must handle `stbi_image_free` (CPU memory) and `glDeleteTextures` (GPU memory).
2.  **Update Geometry:**
    - Modify `vertices[]` in `App.cpp` to include UV coordinates (5 floats per vertex).
    - Update `glVertexAttribPointer` stride and offsets.
3.  **Update Shaders:**
    - Vertex Shader: Pass `vec2 TexCoord` to Fragment Shader.
    - Fragment Shader: Use `sampler2D` to output texture color.
