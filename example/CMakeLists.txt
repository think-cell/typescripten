cmake_minimum_required(VERSION 3.21)

project(
  typescripten_test
  VERSION 0.9
  LANGUAGES C CXX
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)
FetchContent_Declare(
  typescripten
  GIT_REPOSITORY https://github.com/think-cell/typescripten
)
FetchContent_MakeAvailable(typescripten)
include(${typescripten_SOURCE_DIR}/cmake/TypeScripten.cmake)
execute_process(COMMAND npm install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(typescripten_test main.cpp)

add_typescripten_target(
  TARGET typescripten_test
  INPUTS ${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.dom.d.ts
  OUTPUT lib.dom.d.h
)

target_compile_options(typescripten_test PRIVATE "-Wall" "-Wextra" "-Wno-unused-parameter")
target_link_options(typescripten_test PRIVATE "--bind")
