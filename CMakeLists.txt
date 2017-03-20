cmake_minimum_required(VERSION 3.6)
project(ArcV)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ArcV)
set(SOURCE_FILES src/ArcV/Image.cpp)

#add_library(ArcV SHARED ${SOURCE_FILES})
add_executable(ArcV ${SOURCE_FILES} main.cpp)

target_link_libraries(ArcV ${LIBS}
						png16
						jpeg)
