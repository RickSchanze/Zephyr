project(Widgets)

add_library(${PROJECT_NAME}
        WidgetBase.h
        WidgetBase.cpp
        BaseWindow.h
        BaseWindow.cpp
        ConsoleWindow.h
        ConsoleWindow.cpp
        ViewportWindow.h
        ViewportWindow.cpp
)

target_link_libraries(${PROJECT_NAME}
        imgui
        spdlog
        Engine::Core
        Engine::Platform
)
