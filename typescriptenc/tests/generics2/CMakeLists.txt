set(TEST_NAME generics2)
add_executable(${TEST_NAME} main.cpp)
target_compile_options(${TEST_NAME} PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})
target_link_options(${TEST_NAME} PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS})

include(${CMAKE_SOURCE_DIR}/cmake/TypeScripten.cmake)
add_typescripten_target(
    TARGET ${TEST_NAME}
    OUTPUT MyLib.d.h
    INPUTS ${CMAKE_CURRENT_SOURCE_DIR}/MyLib.d.ts
)

# add_test(NAME ${TEST_NAME} COMMAND node ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.js)