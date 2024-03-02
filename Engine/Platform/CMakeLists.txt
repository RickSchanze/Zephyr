project(Platform)

add_library(${PROJECT_NAME}
        OpenGL/VertexArrayObject.h
        OpenGL/VertexArrayObject.cpp
        OpenGL/VertexBufferObject.h
        OpenGL/VertexBufferObject.cpp
        OpenGL/FrameBufferObject.h
        OpenGL/FrameBufferObject.cpp
        OpenGL/ElementBufferObject.h
        OpenGL/ElementBufferObject.cpp
        OpenGL/RenderBufferObject.h
        OpenGL/RenderBufferObject.cpp
        OpenGL/Texture.h
        OpenGL/Texture.cpp
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        glad
        Engine::Core
)
