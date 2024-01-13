cmake_minimum_required (VERSION 3.10)

project (GB39 C)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES "*.c")
add_executable (GB39 ${SOURCES})
target_link_libraries(GB39 ${SDL2_LIBRARIES})
