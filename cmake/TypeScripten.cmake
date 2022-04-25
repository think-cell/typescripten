function(add_typescripten_target)
    set(oneValueArgs TARGET OUTPUT WORKING_DIRECTORY)
    set(multiValueArgs INPUTS DEPENDSON)
    cmake_parse_arguments(TSTARGET "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if(NOT TSTARGET_WORKING_DIRECTORY)
        set(TSTARGET_WORKING_DIRECTORY ${typescripten_BINARY_DIR}/typescriptenc)
    endif()

    if (CMAKE_HOST_WIN32)
        set(RUNNODE runnode.cmd)
    else()
        set(RUNNODE runnode.sh)
    endif()

    set(RUN_TYPESCRIPTEN_DIR "${PROJECT_BINARY_DIR}/run_typescripten_${TSTARGET_TARGET}")

    set(TSTARGET_DEFAULT_INPUTS 
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es5.d.ts" 
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.core.d.ts"
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.collection.d.ts"
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.iterable.d.ts"
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.generator.d.ts"
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.promise.d.ts"
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.proxy.d.ts"
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.reflect.d.ts"
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.symbol.d.ts"
        "${PROJECT_SOURCE_DIR}/node_modules/typescript/lib/lib.es2015.symbol.wellknown.d.ts"
    )

    add_custom_command(
        OUTPUT ${RUN_TYPESCRIPTEN_DIR}/${TSTARGET_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RUN_TYPESCRIPTEN_DIR}
        COMMAND ${typescripten_SOURCE_DIR}/cmake/${RUNNODE} ${typescripten_BINARY_DIR}/typescriptenc/typescriptenc.js ${RUN_TYPESCRIPTEN_DIR}/${TSTARGET_OUTPUT} ${TSTARGET_DEFAULT_INPUTS} ${TSTARGET_INPUTS} 
        DEPENDS typescriptenc ${TSTARGET_DEPENDSON} ${TSTARGET_DEFAULT_INPUTS} ${TSTARGET_INPUTS}
        WORKING_DIRECTORY ${TSTARGET_WORKING_DIRECTORY}
    )
    target_sources(${TSTARGET_TARGET} PRIVATE ${RUN_TYPESCRIPTEN_DIR}/${TSTARGET_OUTPUT})

    target_include_directories(${TSTARGET_TARGET} AFTER PRIVATE ${RUN_TYPESCRIPTEN_DIR})
    target_link_libraries(${TSTARGET_TARGET} typescripten)

    target_compile_options(${TSTARGET_TARGET} PRIVATE "-DBOOST_HANA_CONFIG_ENABLE_STRING_UDL")
    target_link_options(${TSTARGET_TARGET} PRIVATE "-lembind")
endfunction()