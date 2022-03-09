cmake_minimum_required(VERSION 3.5)

project(RanokLanguage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(THIRDPARTY_DIR ThirdParty)
set(ASSETS_DIR CodeExamples)

add_subdirectory(${THIRDPARTY_DIR}/fmt)

file(GLOB_RECURSE PROJECT_SOURCES Sources/*.cpp Sources/*.h Sources/*.hpp ${ASSETS_DIR}/*)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} fmt)
