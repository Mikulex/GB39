cmake_minimum_required (VERSION 3.26.4)

project (GB39 C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES "*.c")
add_executable (GB39 WIN32 ${SOURCES})
target_link_libraries(GB39 ${SDL2_LIBRARIES})
