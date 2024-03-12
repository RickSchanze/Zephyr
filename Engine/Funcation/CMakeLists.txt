project(Function)

add_library(${PROJECT_NAME}
        Render/Renderer/StaticMeshRenderer.h
        Render/Renderer/StaticMeshRenderer.cpp
        Render/Renderer/BaseRenderer.h
        Render/Renderer/BaseRenderer.cpp
        Render/Renderer/FrameRenderer.h
        Render/Renderer/FrameRenderer.cpp
        Object/Object.cpp
        Object/Object.h
        Components/ComponentBase.h
        Components/ComponentBase.cpp
        Components/RendererComponentBase.h
        Components/RendererComponentBase.cpp
        Components/StaticModelRendererComponent.h
        Components/StaticModelRendererComponent.cpp
)

target_link_libraries(${PROJECT_NAME}
        Engine::Core
        Engine::Platform
        Engine::Resource
        glm
        imgui
)

add_library(Engine::Function ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
