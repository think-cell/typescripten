set(TEST_NAME BootstrapDemo)

add_executable(${TEST_NAME} main.cpp)
target_compile_options(${TEST_NAME} PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})
target_link_options(${TEST_NAME} PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS})

include(${PROJECT_SOURCE_DIR}/cmake/TypeScripten.cmake)
add_typescripten_target(
  TARGET ${TEST_NAME}
  INPUTS ${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.dom.d.ts
  OUTPUT lib.dom.d.h
)

target_link_libraries(${TEST_NAME} typescripten)
add_test(NAME ${TEST_NAME} COMMAND node ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.js)