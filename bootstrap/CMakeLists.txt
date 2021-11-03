# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

file(GLOB BOOTSTRAP_INCLUDES CONFIGURE_DEPENDS 
    "include/*.h" 
    "include/*.inl"
    "include/*.ts"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(bootstrap src/js_callback.cpp ${BOOTSTRAP_INCLUDES})

target_compile_options(bootstrap PRIVATE ${TYPESCRIPTEN_COMPILE_OPTIONS})
target_link_options(bootstrap PRIVATE ${TYPESCRIPTEN_LINK_OPTIONS})

# We need this directory, and users of our library will need it too
target_include_directories(bootstrap PUBLIC include ${tcrange_SOURCE_DIR}/tc)

# Link our JS stub
em_link_pre_js(bootstrap src/js_callback.js)

# This depends on (header only) boost
# Cannot specify "PUBLIC" because em_link_pre_js does not
target_link_libraries(bootstrap Boost::boost)
