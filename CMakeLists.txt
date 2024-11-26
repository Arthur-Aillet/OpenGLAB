# CMakeList.txt : CMake project for lab0, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.5)

project(project DESCRIPTION "Exemple OpenGL project" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(glm CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable(project "src/main.cpp" "src/MyGlWindow.h" "src/MyGlWindow.cpp" "src/Loader.h" "src/cube.h" "src/Viewer.cpp" "src/Viewer.h" "src/model.h" "src/model.cpp" "src/Viewer.h")
#raster3d.cpp cow.h ppm_io.cpp ppm_io.h

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET project PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(project PRIVATE glm::glm glfw unofficial::gl3w::gl3w ${OPENGL_INCLUDE_DIRS})
