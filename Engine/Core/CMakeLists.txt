project(Core)

find_package(jsoncpp REQUIRED CONFIG)

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
        Reflection/JsonSerializer.h
        Reflection/JsonSerializer.cpp
        Reflection/TypeTraits.h
        CommonMacro.h
        Coroutine/Test.h
)

target_link_libraries(${PROJECT_NAME}
        spdlog::spdlog
        glm
        utf8cpp
        jsoncpp_lib
        Asio
)

get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})

add_library(Engine::Core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} jsoncpp_lib)

