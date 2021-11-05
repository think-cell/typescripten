add_executable(typescriptenc 
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
target_compile_options(typescriptenc PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})

em_link_pre_js(typescripten main-pre.js)

target_link_options(typescriptenc PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS})
target_link_libraries(typescriptenc typescripten Boost::boost)
