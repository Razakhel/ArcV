cmake_minimum_required(VERSION 3.6)
project(ArcV)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ArcV)
file(
    GLOB_RECURSE
    SOURCE_FILES
    src/*.cpp
)

#add_library(ArcV SHARED ${SOURCE_FILES})
add_executable(ArcV ${SOURCE_FILES} examples/main.cpp)

target_link_libraries(
    ArcV ${LIBS}
    png16
    jpeg
    xcb
)
