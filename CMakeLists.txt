cmake_minimum_required(VERSION 3.15)
project(osu-laser-cpp VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

option(COMPILE_WITH_ALL_FONTS_OPTION "Enable compilation using all osu-resources fonts" OFF)

if (CMAKE_HOST_WIN32)
    set(libconfig_libname "libconfig")
else ()
    set(libconfig_libname "config")
endif ()

#find opengl
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)

find_path(SWRESAMPLE_INCLURE_DIR libswresample/swresample.h)
find_library(SWRESAMPLE_LIBRARY swresample)

find_path(SWSCALE_INCLURE_DIR libswscale/swscale.h)
find_library(SWSCALE_LIBRARY swscale)

#find boost
find_package(Boost 1.65.1 REQUIRED COMPONENTS system filesystem)

#cpp default settings
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

#compiling libconfig
add_subdirectory(libconfig)

#compiling glm
add_subdirectory(glm/glm)
add_library(glm::glm ALIAS glm)

#comliling OpenAL
option(ALSOFT_EXAMPLES OFF)
add_subdirectory(openal-soft)

#def glad sources
set(GLAD_SOURCES glad/src/glad.c)

#adding main sources
file(GLOB_RECURSE SOURCE_FILES "osu/desktop/*")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${GLAD_SOURCES})

#compilation defines
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE STB_IMAGE_IMPLEMENTATION)
if (COMPILE_WITH_ALL_FONTS_OPTION)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE COMPILE_WITH_ALL_FONTS)
    message("Compiling with all fonts")
else ()
    message("Compiling with minimal fonts")
endif ()
unset(COMPILE_WITH_ALL_FONTS_OPTION CACHE)

#include cmake generated headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR})

#include boost
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

#include glm
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC glm/glm)

#include GLFW
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC glfw/include)

#include libconfig
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC libconfig/lib)

#include GLAD
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC glad/include)

#include OpenAL
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC openal-soft/include/AL)

#include ffmpeg
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR} ${SWRESAMPLE_INCLUDE_DIR} ${SWSCALE_INCLURE_DIR})

#link libs
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY} pthread ${libconfig_libname}++ OpenAL
        ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY} ${SWRESAMPLE_LIBRARY} ${SWSCALE_LIBRARY})