cmake_minimum_required(VERSION 3.5)

project(RanokLanguage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(ASSETS_DIR CodeExamples)

file(GLOB_RECURSE PROJECT_SOURCES Sources/*.cpp Sources/*.h Sources/*.hpp ${ASSETS_DIR}/*)

add_executable(RanokLanguage ${PROJECT_SOURCES})
