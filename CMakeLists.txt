cmake_minimum_required(VERSION 3.15)
project(osu-laser-cpp VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

option(COMPILE_WITH_ALL_FONTS_OPTION "Enable compilation using all osu-resources fonts" OFF)

#define libconfig name
if (CMAKE_HOST_WIN32)
    set(libconfig_libname "libconfig")
else ()
    set(libconfig_libname "config")
endif ()

#add testing feature
include(CTest)

#find opengl
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

#find ffmpeg libs
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
find_package(Boost REQUIRED COMPONENTS system filesystem)

#set cpp default settings
set(RESOURCES ${PROJECT_SOURCE_DIR}/osu-resources/osu.Game.Resources)
set(CONFIG_PATH ${PROJECT_BINARY_DIR}/config.cfg)
set(FONTS_PATH ${PROJECT_SOURCE_DIR}/osu-resources/osu.Game.Resources/Fonts/)

#generate headers
configure_file(osu/desktop/platformDependentDefines.h.in ${PROJECT_BINARY_DIR}/platformDependentDefines.h)

#compile GLFW
add_subdirectory(glfw)

#compile libconfig
add_subdirectory(libconfig)

#compile glm
add_subdirectory(glm/glm)
add_library(glm::glm ALIAS glm)

#compile OpenAL
add_subdirectory(openal-soft)

message(STATUS "")
message(STATUS "==== Dependencies configured ====")
message(STATUS "")

#define glad sources
set(GLAD_SOURCES glad/src/glad.c)

#add game sources
file(GLOB_RECURSE SOURCE_FILES "osu/desktop/*/*")

#add game logic library
add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCE_FILES} ${GLAD_SOURCES})

#declare compilation defines
target_compile_definitions(${CMAKE_PROJECT_NAME}_lib PUBLIC GLFW_INCLUDE_NONE STB_IMAGE_IMPLEMENTATION)
if (COMPILE_WITH_ALL_FONTS_OPTION)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_lib PRIVATE COMPILE_WITH_ALL_FONTS)
    message(STATUS "Compiling with all fonts")
else ()
    message(STATUS "Compiling with minimal fonts")
endif ()
unset(COMPILE_WITH_ALL_FONTS_OPTION CACHE)

#include cmake generated headers
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_BINARY_DIR})

#include boost headers
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC ${Boost_LIBRARIES})

#include glm headers
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC glm/glm)

#include GLFW headers
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC glfw/include)

#include libconfig headers
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC libconfig/lib)

#include GLAD headers
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC glad/include)

#include OpenAL headers
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC openal-soft/include/AL)

#include ffmpeg headers
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR} ${SWRESAMPLE_INCLUDE_DIR} ${SWSCALE_INCLURE_DIR})

#link libs
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC ${Boost_LIBRARIES} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY} pthread ${libconfig_libname}++ OpenAL
        ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY} ${SWRESAMPLE_LIBRARY} ${SWSCALE_LIBRARY})

#add game executable
add_executable(${CMAKE_PROJECT_NAME} osu/desktop/Osu.cpp)

#include game library headers
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC osu/desktop)

#link executable with lib
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_PROJECT_NAME}_lib)

#compile tests
if (BUILD_TESTING)
    add_subdirectory(osu/test)
endif ()