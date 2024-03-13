cmake_minimum_required(VERSION 3.26)
project(Zephyr)

if (MSVC)
    add_compile_options("/source-charset:utf-8" "/execution-charset:utf-8")
    add_compile_options("/wd4996")
    add_compile_options("/wd4244")
    add_compile_options("/wd4312")
endif ()

file(GLOB_RECURSE REFLECTED_HEADERS Engine/*.h Editor/*.h)

set(REFLECTION_EXE "C:/Users/Echo/BaiduSyncDisk/Work/Projects/Zephyr/Reflection/Release/Reflection.exe")

# 生成反射文件
foreach (HEADER ${REFLECTED_HEADERS})
    execute_process(COMMAND ${REFLECTION_EXE} ${HEADER} --)
endforeach ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DZEPHYR_DEBUG")
add_subdirectory(Editor)
add_subdirectory(Engine)
add_subdirectory(ThirdParty)

add_executable(Zephyr
        "main.cpp"
)

target_link_libraries(${PROJECT_NAME} Engine::Core Editor)
