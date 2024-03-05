project(Resource)

add_library(${PROJECT_NAME}
        Geometry/GeometryType.h
        Geometry/GeometryType.cpp
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Engine::Core)
