cmake_minimum_required(VERSION 3.26)
project(Zephyr)

if (MSVC)
    add_compile_options("/source-charset:utf-8" "/execution-charset:utf-8")
endif ()

set(CMAKE_CXX_STANDARD 20)

add_executable(Zephyr main.cpp)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(Zephyr PRIVATE fmt::fmt)

find_package(glad CONFIG REQUIRED)
target_link_libraries(Zephyr PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui glfw)

file(GLOB_RECURSE SRC_FILES ./Editor/*.cpp ./Engine/*.cpp)
file(GLOB_RECURSE HEADER_FILES ./Editor*.h ./Editor/*.h)

target_sources(Zephyr PRIVATE ${SRC_FILES} ${HEADER_FILES})
