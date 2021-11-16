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

  # See README.md for an explanation on how to setup interactive debugging in Chrome
  option(DEBUG_DEVTOOLS "Build for interactive debugging with Chrome DevTools" OFF)
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

execute_process(COMMAND npm install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TYPESCRIPTEN_COMPILE_OPTIONS 
  "-Wall" "-Wextra" "-fdiagnostics-absolute-paths" "-fno-char8_t" "-Wno-unused-parameter"
  "$<$<CONFIG:DEBUG>:-D_DEBUG;-g;>"
  "$<$<CONFIG:RELEASE>:-Oz;-flto;-gseparate-dwarf;>"
  "$<$<BOOL:${DEBUG_DEVTOOLS}>:-DDEBUG_DEVTOOLS>"
)

set(TYPESCRIPTEN_LINK_OPTIONS 
  "-fno-char8_t" "--bind" "SHELL:-s DEMANGLE_SUPPORT=1"
  "$<$<CONFIG:DEBUG>:-g;SHELL:-s SAFE_HEAP=1;SHELL:-s ASSERTIONS=2;>"
  "$<$<CONFIG:RELEASE>:-Oz;-flto;-gseparate-dwarf;>"
)

# Build the support library. Any user of typescripten generated headers has to link against typescripten target
add_subdirectory(typescripten)

# Build the typescripten compiler. 
add_subdirectory(typescriptenc)

if(DEBUG_DEVTOOLS)
  add_custom_target(
    typescriptenc_debug 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_SOURCE_DIR}/debug
      ${CMAKE_BINARY_DIR}/debug
    COMMAND npx webpack --config ${CMAKE_BINARY_DIR}/debug/webpack.config.js
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Open http://localhost:8000/debug/ in Google Chrome to debug the typescriptenc compiler."
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Edit debug/index.html or ${CMAKE_BINARY_DIR}/debug/index.html to debug typescriptenc with different input."
    COMMAND python3 -m http.server
    DEPENDS ${CMAKE_BINARY_DIR}/typescriptenc/typescriptenc.js
    USES_TERMINAL
  )
endif()

# Testing only available if this is the main app
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

  macro(get_subdirectories result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
      if(IS_DIRECTORY ${curdir}/${child})
          set(dirlist ${dirlist} ${curdir}/${child})
      endif()
    endforeach()
    set(${result} ${dirlist})
  endmacro()
  
  get_subdirectories(TYPESCRIPTEN_TESTS ${CMAKE_SOURCE_DIR}/typescripten/tests)
  foreach(subdir ${TYPESCRIPTEN_TESTS})
      add_subdirectory(${subdir})
  endforeach()

  get_subdirectories(TYPESCRIPTENC_TESTS ${CMAKE_SOURCE_DIR}/typescriptenc/tests)
  foreach(subdir ${TYPESCRIPTENC_TESTS})
      add_subdirectory(${subdir})
  endforeach()
endif()
