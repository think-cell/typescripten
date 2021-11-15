function(add_typescripten_target)
    set(oneValueArgs TARGET OUTPUT)
    set(multiValueArgs INPUTS DEPENDSON)
    cmake_parse_arguments(TSTARGET "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    IF (WIN32)
        set(RUNNODE runnode.cmd)
    ELSE()
        set(RUNNODE runnode.sh)
    ENDIF()

    add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/run_typescripten/${TSTARGET_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/run_typescripten/
        COMMAND ${typescripten_SOURCE_DIR}/cmake/${RUNNODE} ${typescripten_BINARY_DIR}/typescriptenc/typescriptenc.js ${PROJECT_BINARY_DIR}/run_typescripten/${TSTARGET_OUTPUT} ${TSTARGET_INPUTS} 
        DEPENDS typescriptenc ${TSTARGET_DEPENDSON}
    )
    target_sources(${TSTARGET_TARGET} PRIVATE ${PROJECT_BINARY_DIR}/run_typescripten/${TSTARGET_OUTPUT})

    target_include_directories(${TSTARGET_TARGET} AFTER PRIVATE ${PROJECT_BINARY_DIR}/run_typescripten/)
    target_link_libraries(${TSTARGET_TARGET} typescripten)
endfunction()