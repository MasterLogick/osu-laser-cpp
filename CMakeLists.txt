cmake_minimum_required(VERSION 3.15)
project(osu-laser-cpp VERSION 0.0.1 LANGUAGES C CXX)

if (CMAKE_HOST_WIN32)
    set(libconfig_libname "libconfig")
else ()
    set(libconfig_libname "config")
endif ()
#find opengl
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

#find boost
find_package(Boost 1.65.1 REQUIRED COMPONENTS system filesystem)

#cpp default settings
set(CMAKE_CXX_STANDARD 20)
set(RESOURCES ${PROJECT_SOURCE_DIR}/osu-resources/osu.Game.Resources)
set(CONFIG_PATH ${PROJECT_BINARY_DIR}/config.cfg)
set(FONTS_PATH ${PROJECT_SOURCE_DIR}/osu-resources/osu.Game.Resources/Fonts/)

#code generation
configure_file(osu/desktop/platformDependentDefines.h.in ${PROJECT_BINARY_DIR}/platformDependentDefines.h)

# compiling GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(glfw)

#compilation defines
add_compile_definitions(GLFW_INCLUDE_NONE GLM_FORCE_messageS)

#compiling libconfig
add_subdirectory(libconfig)

#compiling glm
add_subdirectory(glm/glm)
add_library(glm::glm ALIAS glm)

#compiling SOIL
add_subdirectory(SOIL)

#adding sources
file(GLOB_RECURSE SOURCE_FILES "osu/desktop/*")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} glad/src/glad.c)#

#include cmake generated headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR})

#include boost
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

#include glm
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC glm/glm)

#including GLFW
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC glfw/include)

#including libconfig
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC libconfig/lib)

#including GLAD
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC glad/include)

#including SIOL
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC SOIL/src)

#link boost GLFW
target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY} pthread ${libconfig_libname}++ soil_static)
