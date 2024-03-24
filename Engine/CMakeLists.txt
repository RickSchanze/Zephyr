project(Engine)

enable_testing()

include_directories(${GENERATED_HEADER_PATH})

add_subdirectory(Core)
add_subdirectory(Platform)
add_subdirectory(Resource)
add_subdirectory(Function)

