project(Core)

add_library(${PROJECT_NAME}
        Event/Event.h
        Utils/Guid.h
        Utils/Guid.cpp
        Utils/TypeConverter.h
        Utils/TypeConverter.cpp
        Utils/Singleton.h
        Logger/Logger.h
        Logger/Logger.cpp
        Logger/Formatter.h
        Math/BasicType.h
        Reflection/MetaType.h
        Reflection/MetaType.cpp
        CommonMacro.h
)

target_link_libraries(${PROJECT_NAME}
        spdlog::spdlog
        glm
        utf8cpp
)

add_library(Engine::Core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
