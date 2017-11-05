cmake_minimum_required(VERSION 3.6)
project(ArcV_Examples)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-value -Wno-unused-parameter -Wno-unused-function")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(colorspacesDemo colorspacesDemo.cpp)
target_link_libraries(colorspacesDemo ArcV)

add_executable(filtersDemo filtersDemo.cpp)
target_link_libraries(filtersDemo ArcV)

add_executable(detectorsDemo detectorsDemo.cpp)
target_link_libraries(detectorsDemo ArcV)

if (UNIX)
    add_executable(webcamDemo webcamDemo.cpp)
    target_link_libraries(webcamDemo ArcV)
endif ()
