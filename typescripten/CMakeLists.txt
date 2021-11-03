add_executable(typescripten 
    intrusive_set.h
    jstypes.cpp
    jstypes.h
    main.cpp
    mangle.cpp
    mangle.h
    mangle.inl
    precompiled.h
    walk_symbol.h
)
target_compile_options(typescripten PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})

em_link_pre_js(typescripten main-pre.js)

target_link_options(typescripten PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS})
target_link_libraries(typescripten bootstrap Boost::boost)
