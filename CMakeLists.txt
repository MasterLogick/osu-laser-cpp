cmake_minimum_required(VERSION 3.15)
project(osu-laser-cpp)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# Including GLFW
add_subdirectory(glfw)
include_directories(glfw/include)

# Including GLAD
include_directories(glad/include)

include_directories(libconfig)

#set(SOURCE_FILES osu/desktop/main.cpp glad/src/glad.c osu/desktop/graphics/GLFWWindow.h osu/desktop/interaction/Properties.h osu/desktop/interaction/Property.cpp osu/desktop/interaction/Property.h osu/desktop/graphics/GLFWWindow.cpp)
file(GLOB_RECURSE SOURCE_FILES "osu/desktop/*")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# Linking GLFW and OGL
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
