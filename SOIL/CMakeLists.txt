cmake_minimum_required(VERSION 3.15)
project(SOIL LANGUAGES CXX)

file(GLOB SOIL_SOURCE_FILES "src/*")
add_library(soil-static STATIC ${SOIL_SOURCE_FILES})
add_library(soil-shared SHARED ${SOIL_SOURCE_FILES})
