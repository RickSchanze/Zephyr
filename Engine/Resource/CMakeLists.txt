project(Resource)

add_library(${PROJECT_NAME}
        Geometry/Mesh.h
        Geometry/Mesh.cpp
        Geometry/Image.h
        Geometry/Image.cpp
        Geometry/Model.h
        Geometry/Model.cpp
)

add_library(Engine::Resource ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Engine::Core
        stbimage
        assimp
        utf8cpp
)
