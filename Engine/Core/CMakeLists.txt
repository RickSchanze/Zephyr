project(Core)

set(LIB_FILES
        CommonMacro.h
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
        Reflection/JsonSerializer.h
        Reflection/JsonSerializer.cpp
        Reflection/TypeTraits.h
        Reflection/JsonDeserializer.cpp
        Reflection/JsonDeserializer.h
        Coroutine/Coroutine.h
        Object/Object.h
        Object/Object.cpp

        ReflectionClasses.h
        ReflectionClasses.cpp
)

add_library(${PROJECT_NAME}
    ${LIB_FILES}
)

target_link_libraries(${PROJECT_NAME}
        spdlog::spdlog
        glm
        utf8cpp
        jsoncpp_static
        Asio
)

add_library(Engine::Core ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

