cmake_minimum_required(VERSION 3.20)
project(Reflection)

# llvm
find_package(LLVM REQUIRED CONFIG)
# clang
find_package(Clang REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options("/source-charset:utf-8" "/execution-charset:utf-8")
    add_compile_options("/wd4996")
    add_compile_options("/wd4291")
endif ()

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(Reflection
        main.cpp
        Finder.h
        Finder.cpp
        ReflectedClass.h
        ReflectedClass.cpp
)

target_link_libraries(Reflection
    clangAST
    clangBasic
    clangTooling
    clangASTMatchers)
