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

include(FetchContent)
FetchContent_Declare(
  cppunicodelib
  GIT_REPOSITORY https://github.com/yhirose/cpp-unicodelib
)
FetchContent_MakeAvailable(cppunicodelib)

# Building without exceptions
target_compile_options(typescriptenc PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})
target_include_directories(typescriptenc PRIVATE ${cppunicodelib_SOURCE_DIR})

em_link_pre_js(typescriptenc main-pre.js)

target_link_options(typescriptenc PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS} "-sENVIRONMENT=node")
target_link_libraries(typescriptenc typescripten Boost::boost)
