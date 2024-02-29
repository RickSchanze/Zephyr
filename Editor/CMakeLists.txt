project(Editor)

add_subdirectory(Widgets)

add_library(${PROJECT_NAME}
        Application.h
        Application.cpp
)

target_link_libraries(${PROJECT_NAME}
        glad
        Engine::Core
        glfw
        imgui
        Widgets
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}
)


