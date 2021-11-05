cmake_minimum_required(VERSION 3.21)

project(
  typescripten
  VERSION 0.9
  LANGUAGES C CXX
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)
FetchContent_Declare(
  tcrange
  GIT_REPOSITORY https://github.com/think-cell/range/
  GIT_TAG        clang_12_cpp2a
)
FetchContent_MakeAvailable(tcrange)

find_package(Boost 1.73 REQUIRED)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TYPESCRIPTEN_COMPILE_OPTIONS 
  "-Wall" "-Wextra" "-fdiagnostics-absolute-paths" "-fno-char8_t" "-Wno-unused-parameter"
  "$<$<CONFIG:DEBUG>:-D_DEBUG;-gsource-map;>"
  "$<$<CONFIG:RELEASE>:-Oz;-flto;-g0;>"
)

set(TYPESCRIPTEN_LINK_OPTIONS 
  "-fno-char8_t" "--bind" "SHELL:-s DEMANGLE_SUPPORT=1"
  "$<$<CONFIG:DEBUG>:-gsource-map;--source-map-base;http://localhost:8000/;SHELL:-s SAFE_HEAP=1;SHELL:-s ASSERTIONS=2;>"
  "$<$<CONFIG:RELEASE>:-Oz;-flto;-g0;--emit-symbol-map;>"
)

# Build the support library. Any user of typescripten generated headers has to link against typescripten target
add_subdirectory(typescripten)

# Build the typescripten compiler. 
add_custom_target(install_node_packages npm install)
add_subdirectory(typescriptenc)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
#if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
#   AND BUILD_TESTING)
#  add_subdirectory(bootstrap/tests)
#  add_subdirectory(typescripten/tests)
#endif()
