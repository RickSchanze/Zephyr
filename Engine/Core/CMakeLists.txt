project(Core)

add_library(${PROJECT_NAME}
        Event/Event.h
        Utils/Guid.h
        Utils/Guid.cpp
        Utils/TypeConverter.h
        Utils/TypeConverter.cpp
        Logger/Logger.h
        Logger/Logger.cpp
        Logger/Formatter.h
        Renderer/BaseRenderer.h
        Renderer/BaseRenderer.cpp
        Renderer/FrameRenderer.h
        Renderer/FrameRenderer.cpp
        Renderer/StaticModelRenderer.h
        Renderer/StaticModelRenderer.cpp
        Math/BasicType.h
)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog Platform glm)

add_library(Engine::Core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
