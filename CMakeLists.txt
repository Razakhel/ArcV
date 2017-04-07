cmake_minimum_required(VERSION 3.6)
project(ArcV)

set(CMAKE_CXX_STANDARD 14)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/png
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(
    GLOB_RECURSE
    SOURCE_FILES
    extern/*.c
    src/ArcV/Math/*.cpp
    src/ArcV/Processing/*.cpp
    #src/ArcV/Utils/*.cpp

    include/ArcV/ArcV.hpp
    include/ArcV/Math/*.hpp
    include/ArcV/Math/*.inl
    include/ArcV/Processing/*.hpp
    include/ArcV/Processing/*.inl
    #include/ArcV/Utils/*.hpp
    #include/ArcV/Utils/*.inl
)

#add_library(ArcV SHARED ${SOURCE_FILES})
add_executable(ArcV ${SOURCE_FILES} examples/main.cpp)

target_link_libraries(
    ArcV ${LIBS}
    #xcb
    #xcb-image
)
