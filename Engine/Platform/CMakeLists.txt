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
        OpenGL/FrameBufferObject.h
        OpenGL/FrameBufferObject.cpp
        OpenGL/Shader.h
        OpenGL/Shader.cpp
        OpenGL/ShaderProgram.h
        OpenGL/ShaderProgram.cpp
)

target_link_libraries(${PROJECT_NAME}
        glad
        Engine::Core
)

add_library(Engine::Platform ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
