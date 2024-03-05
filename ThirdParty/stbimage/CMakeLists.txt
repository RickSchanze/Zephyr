project(stbimage)

add_library(${PROJECT_NAME} INTERFACE stb_image.h)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
