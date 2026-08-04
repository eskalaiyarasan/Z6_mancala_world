add_executable(engine
    *.cpp
    ../pipeline/*.cpp
    ../json/*.cpp
    ../utils/*.cpp
)

target_include_directories(engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../json
    ${CMAKE_CURRENT_SOURCE_DIR}/../pipeline
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
)

# Places the final executable 'A' inside .build/A/
set_target_properties(engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/.build/debug/engine"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/.build/release/engine"
)
