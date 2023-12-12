cmake_minimum_required(VERSION 3.16)

# Sets project name
project(projects_realtime LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Sets C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specifies required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Xml)

# Allows you to include files from within those directories, without prefixing their filepaths
include_directories(src)

# Specifies .cpp and .h files to be passed to the compiler
add_executable(${PROJECT_NAME}
    src/main.cpp

    src/realtime.cpp
    src/mainwindow.cpp
    src/settings.cpp
    src/utils/scenefilereader.cpp
    src/utils/sceneparser.cpp

    src/mainwindow.h
    src/realtime.h
    src/settings.h
    src/utils/scenedata.h
    src/utils/scenefilereader.h
    src/utils/sceneparser.h
    src/utils/shaderloader.h
    src/utils/aspectratiowidget/aspectratiowidget.hpp
    src/utils/terraingenerator.h
    src/utils/terraingenerator.cpp
    src/camera.h src/camera.cpp
    src/shapes/cube.h src/shapes/cube.cpp
    src/shapes/cone.h src/shapes/cone.cpp
    src/shapes/cylinder.h src/shapes/cylinder.cpp
    src/shapes/sphere.h src/shapes/sphere.cpp
    src/scene.h src/scene.cpp

    src/debug.h
    src/shaders/openglwrappers.h
    src/shaders/phong/phongvbo.h src/shaders/phong/phongvbo.cpp
    src/shapes/shape.h src/shapes/shape.cpp
    src/shaders/phong/phongvao.h src/shaders/phong/phongvao.cpp
    src/shaders/phong/phongshader.h src/shaders/phong/phongshader.cpp
    src/shaders/fbo.h src/shaders/fbo.cpp
    src/shaders/shader.h src/shaders/shader.cpp
    src/shaders/vao.h src/shaders/vao.cpp
    src/shaders/vbo.h src/shaders/vbo.cpp
    src/shaders/texture/textureshader.h src/shaders/texture/textureshader.cpp
    src/shaders/texture/texturevbo.h src/shaders/texture/texturevbo.cpp
    src/shaders/texture/texturevao.h src/shaders/texture/texturevao.cpp
    src/shaders/skybox/skyboxshader.h src/shaders/skybox/skyboxshader.cpp
    src/shaders/skybox/skyboxvbo.h src/shaders/skybox/skyboxvbo.cpp
    src/shaders/skybox/skyboxvao.h src/shaders/skybox/skyboxvao.cpp


)

# GLM: this creates its library and allows you to `#include "glm/..."`
add_subdirectory(glm)

# GLEW: this creates its library and allows you to `#include "GL/glew.h"`
add_library(StaticGLEW STATIC glew/src/glew.c
    src/bezier.h src/bezier.cpp)
include_directories(${PROJECT_NAME} PRIVATE glew/include)

# Specifies libraries to be linked (Qt components, glew, etc)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    Qt::Xml
    StaticGLEW
)

# Specifies other files
qt6_add_resources(${PROJECT_NAME} "Resources"
    PREFIX
        "/"
    FILES
        resources/shaders/phong.frag
        resources/shaders/phong.vert
        resources/shaders/texture.frag
        resources/shaders/texture.vert
        resources/shaders/skybox.frag
        resources/shaders/skybox.vert
)

# GLEW: this provides support for Windows (including 64-bit)
if (WIN32)
  add_compile_definitions(GLEW_STATIC)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32
    glu32
  )
endif()

# Set this flag to silence warnings on Windows
if (MSVC OR MSYS OR MINGW)
  set(CMAKE_CXX_FLAGS "-Wno-volatile")
endif()
# Set this flag to silence warnings on MacOS
if (APPLE)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated-volatile")
endif()
