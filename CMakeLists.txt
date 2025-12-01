cmake_minimum_required(VERSION 3.11)
project(ModelViewer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

# --- Fetch GLFW ---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

# --- Fetch GLM ---
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# --- Fetch Assimp ---
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v5.4.1
)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "")
FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking
)
FetchContent_MakeAvailable(imgui)

find_package(OpenGL REQUIRED)

# --- Platform-specific libraries for GLFW/ImGui ---
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    set(PLATFORM_LIBS
        X11::X11
        ${CMAKE_DL_LIBS}  # for dlopen, dlsym, etc.
    )
elseif(APPLE)
    set(PLATFORM_LIBS
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
    )
elseif(WIN32)
    # Windows doesn't need extra libs beyond what GLFW handles
    set(PLATFORM_LIBS "")
endif()

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL ${PLATFORM_LIBS})

# --- Add Executable ---
add_executable(main
    src/main.cpp
    src/App.cpp
    src/Config.cpp
    src/Core/Window.cpp
    src/Core/Input.cpp
    src/Graphics/Renderer.cpp
    src/Graphics/Shader.cpp
    src/Graphics/Texture.cpp
    src/Graphics/Mesh.cpp
    src/Graphics/ResourceManager.cpp
    src/Graphics/stb_image.cpp
    src/Scene/Model.cpp
    vendor/glad/src/glad.c
)

# --- Include Directories ---
target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/stb
    ${PROJECT_SOURCE_DIR}/vendor/nlohmann
)

# --- Link Libraries to Executable ---
target_link_libraries(main PRIVATE
    glfw           
    glm            
    assimp::assimp 
    OpenGL::GL     
    imgui
    ${PLATFORM_LIBS}
)