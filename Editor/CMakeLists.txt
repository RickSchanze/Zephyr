project(Editor)

add_library(${PROJECT_NAME}
        Application.h
        Application.cpp
)

target_link_libraries(${PROJECT_NAME}
        glad
        Engine::Core
        glfw
        imgui
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}
)


