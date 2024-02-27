cmake_minimum_required(VERSION 3.26)
project(Zephyr)

if (MSVC)
    add_compile_options("/source-charset:utf-8" "/execution-charset:utf-8")
endif ()

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "Engine/*.cpp" "Editor/*.cpp")
file(GLOB HEADERS "Engine/*.h" "Editor/*.h")

add_executable(Zephyr
    ${SOURCES}
    ${HEADERS}
    "main.cpp"
        Engine/Core/Utils/Guid.cpp
)

include_directories(Engine)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(Zephyr PRIVATE fmt::fmt)

find_package(glad CONFIG REQUIRED)
target_link_libraries(Zephyr PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui glfw)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

