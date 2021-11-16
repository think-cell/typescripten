set(TEST_NAME run-browser-global)
add_executable(${TEST_NAME} main.cpp)
target_compile_options(${TEST_NAME} PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})
target_link_options(${TEST_NAME} PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS})

include(${CMAKE_SOURCE_DIR}/cmake/TypeScripten.cmake)

execute_process(COMMAND ${CMAKE_SOURCE_DIR}/node_modules/typescript/bin/tsc --strict --declaration ${CMAKE_CURRENT_SOURCE_DIR}/MyLib.ts --outDir ${CMAKE_CURRENT_BINARY_DIR})

# em_link_pre_js(${TEST_NAME} runbrowserglobal.js)
add_typescripten_target(
    TARGET ${TEST_NAME}
    OUTPUT MyLib.d.h
    INPUTS MyLib.d.ts
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# add_test(NAME ${TEST_NAME} COMMAND node ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.js)