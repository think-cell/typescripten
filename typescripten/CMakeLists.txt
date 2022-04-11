# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

file(GLOB TYPESCRIPTEN_INCLUDES CONFIGURE_DEPENDS 
    "typescripten/*.h" 
    "typescripten/*.inl"
    "typescripten/*.ts"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(typescripten src/callback.cpp ${TYPESCRIPTEN_INCLUDES})

target_compile_options(typescripten PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})
target_link_options(typescripten PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS})

# We need this directory, and users of our library will need it too
target_include_directories(typescripten PUBLIC .)
target_include_directories(typescripten SYSTEM PUBLIC ${tcrange_SOURCE_DIR})

# Link our JS stub
em_link_pre_js(typescripten src/callback.js)

# This depends on (header only) boost
# Cannot specify "PUBLIC" because em_link_pre_js does not
target_link_libraries(typescripten Boost::boost)
