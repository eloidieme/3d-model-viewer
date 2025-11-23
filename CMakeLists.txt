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

find_package(OpenGL REQUIRED)

# --- Add Executable ---
add_executable(main
    src/main.cpp
    src/stb_image.cpp
    vendor/glad/src/glad.c
)

# --- Include Directories ---
target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/stb
)

# --- Link Libraries to Executable ---
target_link_libraries(main PRIVATE
    glfw           
    glm            
    assimp::assimp 
    OpenGL::GL     
)

if(APPLE)
    target_link_libraries(main PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
    )
endif()
