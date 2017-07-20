project(untitled)
cmake_minimum_required(VERSION 2.8)


FILE(GLOB_RECURSE hders "*.h")
add_custom_target(headers SOURCES ${hders})
FILE(GLOB_RECURSE srcs "*.cpp")
add_custom_target(sources SOURCES ${srcs})

set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")

add_executable(${PROJECT_NAME} ${srcs})
target_link_libraries( ${PROJECT_NAME}
    Opengl32.lib
    glu32.lib
    dwmapi.lib
)
