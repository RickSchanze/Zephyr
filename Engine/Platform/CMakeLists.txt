project(Platform)

add_library(${PROJECT_NAME}
        OpenGL/VertexArrayObject.h
        OpenGL/VertexArrayObject.cpp
        OpenGL/VertexBufferObject.h
        OpenGL/VertexBufferObject.cpp
        OpenGL/ElementBufferObject.h
        OpenGL/ElementBufferObject.cpp
        OpenGL/RenderBufferObject.h
        OpenGL/RenderBufferObject.cpp
        OpenGL/Texture.h
        OpenGL/Texture.cpp
        OpenGL/Renderer/FrameRenderer.h
        OpenGL/Renderer/FrameRenderer.cpp
        OpenGL/Renderer/StaticMeshRenderer.h
        OpenGL/Renderer/StaticMeshRenderer.cpp
        OpenGL/FrameBufferObject.h
        OpenGL/FrameBufferObject.cpp
        OpenGL/Renderer/BaseRenderer.h
        OpenGL/Renderer/BaseRenderer.cpp
        OpenGL/Shader.h
        OpenGL/Shader.cpp
)

target_link_libraries(${PROJECT_NAME}
        glad
        Engine::Core
)

add_library(Engine::Platform ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
