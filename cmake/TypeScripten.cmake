function(add_typescripten_target)
    set(oneValueArgs TARGET OUTPUT)
    set(multiValueArgs INPUTS DEPENDSON)
    cmake_parse_arguments(TSTARGET "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    add_custom_target(run_typescripten_make_directory
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/run_typescripten/
    )

    IF (WIN32)
        set(RUNNODE runnode.cmd)
    ELSE()
        set(RUNNODE runnode.sh)
    ENDIF()

    add_custom_target(run_typescripten 
        ${typescripten_SOURCE_DIR}/cmake/${RUNNODE} ${typescripten_BINARY_DIR}/typescriptenc/typescriptenc.js ${PROJECT_BINARY_DIR}/run_typescripten/${TSTARGET_OUTPUT} ${TSTARGET_INPUTS} 
        BYPRODUCTS ${PROJECT_BINARY_DIR}/run_typescripten/${TSTARGET_OUTPUT}
    )

    add_dependencies(run_typescripten run_typescripten_make_directory ${TSTARGET_DEPENDSON} typescriptenc)
    add_dependencies(${TSTARGET_TARGET} run_typescripten)

    target_include_directories(${TSTARGET_TARGET} AFTER PRIVATE ${PROJECT_BINARY_DIR}/run_typescripten/)
    target_link_libraries(${TSTARGET_TARGET} typescripten)
endfunction()