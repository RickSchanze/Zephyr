cmake_minimum_required(VERSION 3.26)
project(Zephyr)

#find_package(jsoncpp CONFIG REQUIRED)

if (MSVC)
    add_compile_options("/source-charset:utf-8" "/execution-charset:utf-8")
    add_compile_options("/wd4996")
    add_compile_options("/wd4244")
    add_compile_options("/wd4312")
endif ()

file(GLOB_RECURSE REFLECTED_HEADERS Engine/*.h Editor/*.h)

set(REFLECTION_EXE "C:/Users/Echo/BaiduSyncDisk/Work/Projects/Zephyr/Reflection/cmake-build-release/Reflection.exe")
set(PYTHON_EXE "C:/Users/Echo/AppData/Local/Programs/Python/Python312/python.exe")


execute_process(COMMAND ${PYTHON_EXE} ${CMAKE_SOURCE_DIR}/GenerateReflectionFiles.py
        --reflection-tool-path=${REFLECTION_EXE}
        --output-files-path=${CMAKE_SOURCE_DIR}/Generated
        --process-files-path ${CMAKE_SOURCE_DIR}/Engine ${CMAKE_SOURCE_DIR}/Editor
        --third-party-path=${CMAKE_SOURCE_DIR}/ThirdParty
        --include-path ${CMAKE_SOURCE_DIR}/Engine ${CMAKE_SOURCE_DIR}/Editor
        --special-include-path ${CMAKE_SOURCE_DIR}/Engine/Core ${CMAKE_SOURCE_DIR}/ThirdParty/glm ${CMAKE_SOURCE_DIR}/ThirdParty/utfcpp/source
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DZEPHYR_DEBUG")

include_directories(${JSON_INC_PATH})#通过这个来添加库

include_directories(${CMAKE_SOURCE_DIR}/Generated)

add_subdirectory(Editor)
add_subdirectory(Engine)
add_subdirectory(ThirdParty)

add_executable(Zephyr
        main.cpp
        Engine/Core/ReflectionClasses.h
        Engine/Core/ReflectionClasses.cpp
)

target_link_libraries(${PROJECT_NAME} Engine::Core Editor jsoncpp_static)
