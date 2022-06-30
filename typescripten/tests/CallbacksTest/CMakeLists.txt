set(TEST_NAME CallbacksTest)

add_executable(${TEST_NAME} main.cpp)
target_compile_options(${TEST_NAME} PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})
target_link_options(${TEST_NAME} PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS})

include(${CMAKE_SOURCE_DIR}/cmake/TypeScripten.cmake)
add_typescripten_target(
    TARGET ${TEST_NAME}
    INPUTS "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.d.ts"
    OUTPUT lib.es2015.h
)

em_link_pre_js(${TEST_NAME} callbacks.js)

target_link_libraries(${TEST_NAME} typescripten)
add_test(NAME ${TEST_NAME} COMMAND node ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.js)
