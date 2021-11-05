add_executable(typescriptenc 
    intrusive_set.h
    jstypes.cpp
    jstypes.h
    main.cpp
    mangle.cpp
    mangle.h
    mangle.inl
    walk_symbol.h
)

target_precompile_headers(typescriptenc
  PRIVATE
    <chrono>
    <cmath>
    <string>
    <iostream>
    <variant>
    <vector>
    <utility>
    <optional>
    <tc/conditional_range.h>
    <tc/explicit_cast.h>
    <tc/assert_defs.h>
    <tc/range.h>
    <tc/join_adaptor.h>
    <typescripten/typescripten.h>
    "typescript.d.bootstrap.h"
)

target_compile_options(typescriptenc PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})

em_link_pre_js(typescripten main-pre.js)

target_link_options(typescriptenc PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS})
add_dependencies(typescriptenc install_node_packages)
target_link_libraries(typescriptenc typescripten Boost::boost)
