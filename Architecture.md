# Deltaviewer Architecture Specification

## Overview

This document specifies architectural improvements for Deltaviewer, ordered by **Return on Investment (ROI)** - balancing implementation effort against maintainability, testability, and extensibility gains.

**Current Status**: 7/10 - Solid foundation with clear opportunities for improvement

---

## Improvement Specifications (Ordered by ROI)

### 1. Extract UI Management from App Class

**Priority**: HIGH | **Effort**: LOW | **ROI**: ★★★★★

#### Problem
The `App` class contains 100+ lines of ImGui code mixed with application logic, violating Single Responsibility Principle and making both UI and app logic harder to maintain.

#### Specification

Create `UIManager` class to handle all ImGui rendering:

```cpp
// UI/UIManager.hpp
class UIManager {
public:
    struct UIState {
        bool showSettings = true;
        bool showStats = true;
        bool showClippingPlanes = true;
    };
    
    void init(GLFWwindow* window);
    void shutdown();
    void beginFrame();
    void endFrame();
    
    void renderSettingsPanel(RenderSettings& settings);
    void renderStatsPanel(const FrameStats& stats);
    void renderClippingPanel(std::vector<glm::vec4>& planes);
    
private:
    UIState m_state;
    float m_dpiScale = 1.0f;
};
```

#### Implementation Steps
1. Create `UI/UIManager.hpp` and `UI/UIManager.cpp`
2. Move ImGui initialization from `App` constructor
3. Extract panel rendering methods (settings, stats, clipping)
4. Update `App::run()` to delegate to UIManager
5. Remove ImGui includes from `App.hpp`

#### Success Criteria
- [ ] `App.cpp` reduced by ~100 lines
- [ ] No ImGui code in `App` class
- [ ] UI can be tested independently
- [ ] Easy to add new UI panels

---

### 2. Formalize Input System

**Priority**: HIGH | **Effort**: LOW | **ROI**: ★★★★★

#### Problem
Input handling scattered in `App::processInput()` with manual state tracking for keys (`isTabPressed`, `isRPressed`). No way to rebind controls or support multiple input contexts.

#### Specification

```cpp
// Core/Input/InputSystem.hpp
enum class InputAction {
    MoveForward, MoveBackward, MoveLeft, MoveRight,
    ToggleUI, ReloadShaders, Exit
};

enum class InputContext {
    Game,      // Camera movement enabled
    UI         // Camera disabled, cursor visible
};

class InputSystem {
public:
    void init(GLFWwindow* window);
    void update(); // Call once per frame to update states
    
    // Action queries
    bool isActionActive(InputAction action) const;
    bool isActionJustPressed(InputAction action) const;
    bool isActionJustReleased(InputAction action) const;
    
    // Mouse input
    glm::vec2 getMouseDelta() const;
    glm::vec2 getMousePosition() const;
    
    // Context management
    void setContext(InputContext context);
    InputContext getContext() const;
    
    // Key mapping (future: load from config)
    void mapKey(KeyCode key, InputAction action);
    
private:
    struct ActionState {
        bool current = false;
        bool previous = false;
    };
    
    std::unordered_map<InputAction, ActionState> m_actionStates;
    std::unordered_map<KeyCode, InputAction> m_keyBindings;
    InputContext m_context = InputContext::Game;
    glm::vec2 m_lastMousePos{0.0f};
};
```

#### Default Key Bindings
| Action | Keys |
|--------|------|
| MoveForward | W, Up |
| MoveBackward | S, Down |
| MoveLeft | A, Left |
| MoveRight | D, Right |
| ToggleUI | Tab |
| ReloadShaders | R |
| Exit | Escape |

#### Implementation Steps
1. Create `Core/Input/InputSystem.hpp` and `.cpp`
2. Define `InputAction` and `InputContext` enums
3. Implement action state tracking (current/previous frame)
4. Setup default key bindings in constructor
5. Replace `App::processInput()` with InputSystem calls
6. Remove manual state tracking (`isTabPressed`, etc.)

#### Success Criteria
- [ ] No manual boolean state tracking in App
- [ ] Easy to add new actions
- [ ] Context switching (Game/UI) handled automatically
- [ ] Foundation for rebindable controls

---

### 3. Move Camera and Transform to .cpp Files

**Priority**: HIGH | **Effort**: LOW | **ROI**: ★★★★☆

#### Problem
`Camera.hpp` (99 lines) and `Transform.hpp` (54 lines) are fully header-implemented, increasing compile times and exposing implementation details.

#### Specification

**Camera.hpp** (minimal interface):
```cpp
class Camera {
public:
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 7.5f));
    
    // Getters
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;
    
    // Input processing
    void processMovement(CameraMovement direction, float deltaTime);
    void processRotation(float xOffset, float yOffset);
    
    // Configuration
    void setAspectRatio(float width, float height);
    void setMovementSpeed(float speed);
    void setSensitivity(float sensitivity);
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl; // PIMPL idiom
};
```

**Transform.hpp** (minimal interface):
```cpp
class Transform {
public:
    Transform();
    
    const glm::vec3& getPosition() const;
    const glm::vec3& getRotation() const;
    const glm::vec3& getScale() const;
    
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);
    
    const glm::mat4& getModelMatrix();
    
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};
```

#### Implementation Steps
1. Create `Camera.cpp` and `Transform.cpp`
2. Define internal `Impl` structs with existing members
3. Move method implementations to .cpp files
4. Update CMakeLists.txt to include new .cpp files
5. Measure compile time improvement

#### Success Criteria
- [ ] Header files < 30 lines each
- [ ] Compile time reduced (measure before/after)
- [ ] No behavioral changes
- [ ] Implementation details hidden

---

### 4. Configuration Dependency Injection

**Priority**: MEDIUM | **Effort**: MEDIUM | **ROI**: ★★★★☆

#### Problem
Global static `Config` namespace makes testing difficult, prevents multiple configurations, and creates hidden dependencies.

#### Specification

```cpp
// Core/Config/ApplicationConfig.hpp
struct WindowConfig {
    unsigned int width = 1280;
    unsigned int height = 960;
    std::string title = "Deltaviewer";
};

struct RenderConfig {
    glm::vec4 clearColor = glm::vec4(0.1f, 0.1f, 0.2f, 1.0f);
    glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, 2.0f);
};

struct CameraConfig {
    float movementSpeed = 2.5f;
    float sensitivity = 0.1f;
    float fov = 45.0f;
    glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 7.5f);
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};

struct PathConfig {
    std::filesystem::path defaultModel = "assets/models/backpack/backpack.obj";
    std::filesystem::path shaderVert = "assets/shaders/vert.glsl";
    std::filesystem::path shaderFrag = "assets/shaders/frag.glsl";
    std::filesystem::path planeShaderVert = "assets/shaders/plane_vert.glsl";
    std::filesystem::path planeShaderFrag = "assets/shaders/plane_frag.glsl";
};

class ApplicationConfig {
public:
    WindowConfig window;
    RenderConfig render;
    CameraConfig camera;
    PathConfig paths;
    
    static ApplicationConfig fromJSON(const std::filesystem::path& path);
    void toJSON(const std::filesystem::path& path) const;
};
```

#### Migration Strategy
1. Create new config classes (non-breaking)
2. Load both old and new config systems
3. Update classes one-by-one to accept config params
4. Remove old Config namespace when all migrated

#### Implementation Steps
1. Create `Core/Config/ApplicationConfig.hpp` and `.cpp`
2. Implement JSON serialization using nlohmann::json
3. Update `main.cpp` to create `ApplicationConfig` instance
4. Modify constructors: `Window(const WindowConfig&)`, `Camera(const CameraConfig&)`
5. Pass config through dependency chain
6. Remove `Config.hpp` static globals

#### Success Criteria
- [ ] No global state in Config
- [ ] Can create multiple configs for testing
- [ ] Clear dependency flow (main → App → subsystems)
- [ ] Same JSON format supported

---

### 5. Scene Manager Extraction

**Priority**: MEDIUM | **Effort**: MEDIUM | **ROI**: ★★★★☆

#### Problem
Scene-related logic (model, transform, clipping planes, lighting) mixed with application lifecycle in `App` class.

#### Specification

```cpp
// Scene/SceneManager.hpp
class SceneManager {
public:
    SceneManager(ResourceManager& resourceManager, const CameraConfig& cameraConfig);
    
    // Scene objects
    void loadModel(const std::filesystem::path& path);
    void setModelTransform(const Transform& transform);
    
    // Camera
    Camera& getCamera();
    const Camera& getCamera() const;
    
    // Lighting
    void setLightPosition(const glm::vec3& position);
    glm::vec3 getLightPosition() const;
    
    // Clipping planes
    void addClippingPlane(const glm::vec4& plane);
    void removeClippingPlane(size_t index);
    void updateClippingPlane(size_t index, const glm::vec4& plane);
    const std::vector<glm::vec4>& getClippingPlanes() const;
    
    // Rendering
    void render(Renderer& renderer, Shader& shader);
    void renderClippingVisualizers(Renderer& renderer, Shader& planeShader);
    
private:
    ResourceManager& m_resourceManager;
    Camera m_camera;
    Transform m_modelTransform;
    std::shared_ptr<Model> m_model;
    glm::vec3 m_lightPosition;
    std::vector<glm::vec4> m_clippingPlanes;
    std::unique_ptr<Mesh> m_planeVisualizerMesh;
    
    void createPlaneVisualizerMesh();
};
```

#### Implementation Steps
1. Create `Scene/SceneManager.hpp` and `.cpp`
2. Move model, camera, transform, lighting from `App`
3. Move clipping plane management logic
4. Extract rendering methods from `App::run()`
5. Update `App` to use SceneManager

#### Success Criteria
- [ ] `App` class < 150 lines
- [ ] All scene logic in one place
- [ ] Easy to swap scenes or support multiple scenes
- [ ] Clear separation: App = lifecycle, SceneManager = content

---

### 6. Render Command Abstraction

**Priority**: MEDIUM | **Effort**: MEDIUM | **ROI**: ★★★☆☆

#### Problem
Raw OpenGL calls (`glEnable`, `glBlendFunc`, `glDepthMask`) scattered in `App::run()`, tightly coupling to OpenGL and preventing render API abstraction.

#### Specification

```cpp
// Graphics/RenderCommand.hpp
enum class BlendMode {
    None,
    Alpha,          // (SRC_ALPHA, ONE_MINUS_SRC_ALPHA)
    Additive,       // (SRC_ALPHA, ONE)
    Multiplicative  // (DST_COLOR, ZERO)
};

enum class DepthTest {
    Disabled,
    Less,
    LessOrEqual,
    Greater,
    Always
};

class RenderCommand {
public:
    static void init();
    
    // State management
    static void setClearColor(const glm::vec4& color);
    static void clear(bool color = true, bool depth = true);
    
    static void setBlendMode(BlendMode mode);
    static void setDepthTest(DepthTest test);
    static void setDepthWrite(bool enabled);
    
    static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    
    // Draw commands
    static void drawIndexed(uint32_t indexCount);
    static void drawArrays(uint32_t vertexCount);
};
```

#### OpenGL Implementation

```cpp
// Platform/OpenGL/OpenGLRenderCommand.cpp
void RenderCommand::setBlendMode(BlendMode mode) {
    switch (mode) {
        case BlendMode::None:
            glDisable(GL_BLEND);
            break;
        case BlendMode::Alpha:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        // ... other modes
    }
}
```

#### Implementation Steps
1. Create `Graphics/RenderCommand.hpp`
2. Create `Platform/OpenGL/OpenGLRenderCommand.cpp`
3. Implement all current GL state changes used in App
4. Replace raw GL calls with RenderCommand API
5. Move GL includes to .cpp files only

#### Success Criteria
- [ ] No raw GL calls in `App.cpp`
- [ ] All rendering state changes through RenderCommand
- [ ] Foundation for multi-API support
- [ ] Easier to debug render state

---

### 7. Enhanced Event System

**Priority**: LOW | **Effort**: MEDIUM | **ROI**: ★★★☆☆

#### Problem
Current event system only handles window events. No keyboard, mouse, or application events, leading to tight coupling and callback chains.

#### Specification

```cpp
// Core/Events/EventDispatcher.hpp
class EventDispatcher {
public:
    template<typename EventType>
    using EventCallback = std::function<void(const EventType&)>;
    
    template<typename EventType>
    void subscribe(EventCallback<EventType> callback);
    
    template<typename EventType>
    void dispatch(const EventType& event);
    
private:
    template<typename EventType>
    std::vector<EventCallback<EventType>>& getCallbacks();
    
    std::unordered_map<std::type_index, std::any> m_callbacks;
};
```

#### Event Types

```cpp
// Application events
struct ShaderReloadedEvent {
    std::string shaderName;
    bool success;
};

struct ModelLoadedEvent {
    std::filesystem::path path;
    std::shared_ptr<Model> model;
};

// Input events (optional - InputSystem handles most)
struct KeyPressedEvent {
    KeyCode key;
    bool repeat;
};

struct MouseMovedEvent {
    glm::vec2 position;
    glm::vec2 delta;
};

// Scene events
struct CameraMovedEvent {
    glm::vec3 position;
};

struct ClippingPlaneAddedEvent {
    size_t index;
    glm::vec4 plane;
};
```

#### Implementation Steps
1. Create `Core/Events/EventDispatcher.hpp`
2. Implement type-safe callback storage with `std::any`
3. Define application event structs
4. Integrate into existing event flow
5. Add dispatches for shader reload, model load, etc.

#### Success Criteria
- [ ] Type-safe event handling
- [ ] Any component can subscribe to events
- [ ] Decoupled communication between systems
- [ ] Easy to add new event types

---

### 8. Resource Handle System

**Priority**: LOW | **Effort**: HIGH | **ROI**: ★★★☆☆

#### Problem
Resources (textures, shaders, models) passed as `shared_ptr` everywhere. No automatic cleanup of unused resources. Texture loading bypasses ResourceManager in Model class.

#### Specification

```cpp
// Graphics/Resources/ResourceHandle.hpp
template<typename T>
class ResourceHandle {
public:
    ResourceHandle() = default;
    
    std::shared_ptr<T> get() const;
    bool isValid() const;
    const std::string& id() const;
    
private:
    friend class ResourceManager;
    ResourceHandle(std::weak_ptr<T> resource, std::string id);
    
    std::weak_ptr<T> m_resource;
    std::string m_id;
};

using TextureHandle = ResourceHandle<Texture>;
using ShaderHandle = ResourceHandle<Shader>;
using ModelHandle = ResourceHandle<Model>;
```

#### Updated ResourceManager

```cpp
class ResourceManager {
public:
    TextureHandle loadTexture(const std::filesystem::path& path, TextureType type = TextureType::Diffuse);
    ShaderHandle loadShader(const std::string& name, const std::filesystem::path& vert, const std::filesystem::path& frag);
    ModelHandle loadModel(const std::filesystem::path& path);
    
    void unloadUnused(); // Remove resources with refcount == 0
    size_t getLoadedResourceCount() const;
    
private:
    template<typename T>
    struct ResourceCache {
        std::unordered_map<std::string, std::shared_ptr<T>> resources;
    };
    
    ResourceCache<Texture> m_textures;
    ResourceCache<Shader> m_shaders;
    ResourceCache<Model> m_models;
};
```

#### Implementation Steps
1. Create `ResourceHandle<T>` template class
2. Update ResourceManager to return handles instead of shared_ptr
3. Update Model class to use ResourceManager for all texture loads
4. Add `unloadUnused()` periodic cleanup
5. Refactor all resource users to use handles

#### Success Criteria
- [ ] Type-safe resource handles
- [ ] Automatic cleanup of unused resources
- [ ] All resources go through ResourceManager
- [ ] Can query loaded resource count

---

### 9. Scene Graph Architecture

**Priority**: LOW | **Effort**: HIGH | **ROI**: ★★☆☆☆

#### Problem
No support for hierarchical transforms (parent-child relationships), multiple objects, or scene organization. Everything hardcoded for single model.

#### Specification

```cpp
// Scene/SceneNode.hpp
class SceneNode : public std::enable_shared_from_this<SceneNode> {
public:
    explicit SceneNode(const std::string& name);
    virtual ~SceneNode() = default;
    
    // Hierarchy
    void addChild(std::shared_ptr<SceneNode> child);
    void removeChild(const std::string& name);
    std::shared_ptr<SceneNode> findChild(const std::string& name);
    
    // Transform
    void setLocalTransform(const Transform& transform);
    Transform getLocalTransform() const;
    glm::mat4 getWorldTransform() const;
    
    // Rendering
    virtual void draw(Renderer& renderer, Shader& shader);
    
    // Metadata
    const std::string& getName() const { return m_name; }
    
protected:
    std::string m_name;
    Transform m_localTransform;
    SceneNode* m_parent = nullptr;
    std::vector<std::shared_ptr<SceneNode>> m_children;
};

// Scene/ModelNode.hpp
class ModelNode : public SceneNode {
public:
    ModelNode(const std::string& name, std::shared_ptr<Model> model);
    void draw(Renderer& renderer, Shader& shader) override;
    
private:
    std::shared_ptr<Model> m_model;
};

// Scene/Scene.hpp
class Scene {
public:
    Scene();
    
    std::shared_ptr<SceneNode> getRootNode();
    void addNode(std::shared_ptr<SceneNode> node);
    void removeNode(const std::string& name);
    
    void render(Renderer& renderer, Shader& shader);
    
private:
    std::shared_ptr<SceneNode> m_root;
};
```

#### Use Cases Enabled
- Multiple models in scene
- Hierarchical animations (pending animation system)
- Parent-child transformations (e.g., wheel attached to car)
- Scene serialization/deserialization
- Culling and LOD (future)

#### Implementation Steps
1. Create `SceneNode` base class
2. Create `ModelNode` derived class
3. Implement hierarchy management (add/remove children)
4. Implement world transform calculation (parent chain)
5. Create `Scene` class to manage root node
6. Update SceneManager to use Scene
7. Add support for multiple objects in UI

#### Success Criteria
- [ ] Can add multiple models to scene
- [ ] Parent-child transform propagation works
- [ ] Easy to extend with new node types
- [ ] Foundation for animations and complex scenes

---

### 10. Abstract Rendering API

**Priority**: LOW | **Effort**: VERY HIGH | **ROI**: ★★☆☆☆

#### Problem
Direct OpenGL coupling throughout codebase prevents supporting Vulkan, DirectX, or Metal. Testing requires OpenGL context.

> [!WARNING]
> This is a **major refactoring** effort. Only pursue if you need multi-API support or headless testing. Consider this a long-term goal.

#### Specification

```cpp
// Graphics/RenderAPI/RenderAPI.hpp
class RenderAPI {
public:
    enum class API {
        None = 0,
        OpenGL = 1,
        Vulkan = 2,
        DirectX11 = 3,
        Metal = 4
    };
    
    virtual ~RenderAPI() = default;
    
    virtual void init() = 0;
    virtual void shutdown() = 0;
    
    virtual void setClearColor(const glm::vec4& color) = 0;
    virtual void clear() = 0;
    
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    
    virtual void drawIndexed(const VertexArray& vertexArray) = 0;
    
    static API getAPI() { return s_API; }
    static std::unique_ptr<RenderAPI> create();
    
private:
    static API s_API;
};

// Platform/OpenGL/OpenGLRenderAPI.hpp
class OpenGLRenderAPI : public RenderAPI {
    void init() override;
    void shutdown() override;
    void setClearColor(const glm::vec4& color) override;
    void clear() override;
    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void drawIndexed(const VertexArray& vertexArray) override;
};
```

#### Abstraction Layers Needed
1. **VertexBuffer** - Abstract GPU buffer
2. **IndexBuffer** - Abstract index buffer  
3. **VertexArray** - Abstract vertex layout
4. **Shader** - Already somewhat abstract, needs interface
5. **Texture** - Already somewhat abstract, needs interface
6. **RenderAPI** - Core rendering commands

#### Implementation Steps
1. Define `RenderAPI` interface
2. Create OpenGL implementation maintaining current behavior
3. Abstract `Shader` with interface, create `OpenGLShader`
4. Abstract `Texture` with interface, create `OpenGLTexture`
5. Create `VertexBuffer`, `IndexBuffer`, `VertexArray` abstractions
6. Update `Mesh` to use abstract buffers
7. Update `Renderer` to use `RenderAPI` instead of direct GL
8. Move all OpenGL code to `Platform/OpenGL/`

#### Success Criteria
- [ ] No OpenGL includes outside `Platform/OpenGL/`
- [ ] Can swap rendering backend at compile-time
- [ ] All rendering through RenderAPI interface
- [ ] Foundation for Vulkan implementation

---

## Implementation Roadmap

### Phase 1: Foundation Improvements (1 week)
- [x] Specification complete
- [ ] Extract UI Management (#1)
- [ ] Formalize Input System (#2)
- [ ] Move Camera/Transform to .cpp (#3)

**Expected Impact**: -200 lines in App.cpp, better separation, faster iteration

### Phase 2: Architecture Decoupling (1-2 weeks)
- [ ] Configuration Dependency Injection (#4)
- [ ] Scene Manager Extraction (#5)
- [ ] Render Command Abstraction (#6)

**Expected Impact**: Testable components, clearer dependencies, API-agnostic rendering

### Phase 3: Advanced Features (2-3 weeks)
- [ ] Enhanced Event System (#7)
- [ ] Resource Handle System (#8)
- [ ] Scene Graph Architecture (#9)

**Expected Impact**: Support complex scenes, automatic resource management, event-driven architecture

### Phase 4: Long-term Goals (4+ weeks)
- [ ] Abstract Rendering API (#10)

**Expected Impact**: Multi-API support, headless testing, production-grade architecture

---

## Success Metrics

| Metric | Current | Target (Phase 2) | Target (Phase 3) |
|--------|---------|------------------|------------------|
| App.cpp lines | 303 | <150 | <100 |
| Compile time (clean) | Baseline | -10% | -20% |
| Test coverage | 0% | 30% | 60% |
| Module coupling | High | Medium | Low |
| Header-only classes | 2 | 0 | 0 |

---

## References

- **SOLID Principles**: Single Responsibility, Dependency Inversion
- **Design Patterns**: PIMPL, Facade, Observer (Events), Handle-Body (Resources)
- **Architecture Patterns**: Layered Architecture, Dependency Injection

---

## Notes

> [!IMPORTANT]
> Focus on **Phase 1** first - these are quick wins with immediate value. Don't attempt everything at once.

> [!TIP]
> Each improvement is designed to be **incremental** - implement one at a time, verify, then move to next.
