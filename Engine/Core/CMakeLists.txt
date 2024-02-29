project(Core)

add_library(${PROJECT_NAME}
        Event/Event.h
        Utils/Guid.h
        Utils/Guid.cpp
        Utils/TypeConverter.h
        Utils/TypeConverter.cpp
        Logger/Logger.h
        Logger/Logger.cpp
)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

add_library(Engine::Core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
