
file(GLOB MAIN_SOURCES "*.cpp")
file(GLOB MAIN_HEADERS "*.h")
file(GLOB PIPELINE_SOURCES "../pipeline/*.cpp")
file(GLOB PIPELINE_HEADERS "../pipeline/*.h")
file(GLOB JSON_SOURCES "../json/*.cpp")
file(GLOB JSON_HEADERS "../json/*.h")
file(GLOB UTILS_SOURCES "../utils/*.cpp")
file(GLOB UTILS_HEADERS "../utils/*.h")


add_executable(z6main
    ${UTILS_HEADERS}
    ${UTILS_SOURCES}
    ${JSON_SOURCES}
    ${JSON_HEADERS}
    ${MAIN_SOURCES}
    ${MAIN_HEADERS}
    ${PIPELINE_SOURCES}
    ${PIPELINE_HEADERS}
)

target_include_directories(z6main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../json
    ${CMAKE_CURRENT_SOURCE_DIR}/../pipeline
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
)
target_link_libraries(z6main PRIVATE
    uuid
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator::validator)

# Places the final executable 'A' inside .build/A/
set_target_properties(z6main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/debug/"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/release/"
)
