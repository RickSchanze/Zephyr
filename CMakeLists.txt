cmake_minimum_required(VERSION 3.26)
project(Zephyr)

if (MSVC)
    add_compile_options("/source-charset:utf-8" "/execution-charset:utf-8")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_subdirectory(Editor)
add_subdirectory(Engine)
add_subdirectory(ThirdParty)

add_executable(Zephyr
    "main.cpp"
)

target_link_libraries(${PROJECT_NAME} Engine::Core Editor)


