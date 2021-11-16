function(add_typescripten_target)
    set(oneValueArgs TARGET OUTPUT WORKING_DIRECTORY)
    set(multiValueArgs INPUTS DEPENDSON)
    cmake_parse_arguments(TSTARGET "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if(NOT TSTARGET_WORKING_DIRECTORY)
        set(TSTARGET_WORKING_DIRECTORY ${typescripten_BINARY_DIR}/typescriptenc)
    endif()

    if (WIN32)
        set(RUNNODE runnode.cmd)
    else()
        set(RUNNODE runnode.sh)
    endif()

    set(RUN_TYPESCRIPTEN_DIR "${PROJECT_BINARY_DIR}/run_typescripten_${TSTARGET_TARGET}")

    add_custom_command(
        OUTPUT ${RUN_TYPESCRIPTEN_DIR}/${TSTARGET_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RUN_TYPESCRIPTEN_DIR}
        COMMAND ${typescripten_SOURCE_DIR}/cmake/${RUNNODE} ${typescripten_BINARY_DIR}/typescriptenc/typescriptenc.js ${RUN_TYPESCRIPTEN_DIR}/${TSTARGET_OUTPUT} ${TSTARGET_INPUTS} 
        DEPENDS typescriptenc ${TSTARGET_DEPENDSON}
        WORKING_DIRECTORY ${TSTARGET_WORKING_DIRECTORY}
    )
    target_sources(${TSTARGET_TARGET} PRIVATE ${RUN_TYPESCRIPTEN_DIR}/${TSTARGET_OUTPUT})

    target_include_directories(${TSTARGET_TARGET} AFTER PRIVATE ${RUN_TYPESCRIPTEN_DIR})
    target_link_libraries(${TSTARGET_TARGET} typescripten)
endfunction()