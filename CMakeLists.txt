cmake_minimum_required(VERSION 3.6)
project(ArcV)

set(CMAKE_CXX_STANDARD 14)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/extern
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/deps
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(
    SRC

    extern/png/*.c
    extern/glfw/*.c
    extern/imgui/*.cpp
    src/ArcV/Math/*.cpp
    src/ArcV/Processing/*.cpp
    src/ArcV/Utils/*.cpp

    include/ArcV/ArcV.hpp
    include/ArcV/Math/*.hpp
    include/ArcV/Math/*.inl
    include/ArcV/Processing/*.hpp
    include/ArcV/Processing/*.inl
    include/ArcV/Utils/*.hpp
    include/ArcV/Utils/*.inl
)

# Defining preprocessor macros and selecting files to be removed
if (UNIX)
    add_definitions(-D_GLFW_X11)
    add_definitions(-D_GLFW_HAS_XF86VM)

    file(
        GLOB_RECURSE
        DISPOSABLE

        extern/glfw/src/cocoa*
        extern/glfw/src/win32*
    )
elseif (WIN32)
    add_definitions(-D_GLFW_WIN32)

    file(
        GLOB_RECURSE
        DISPOSABLE

        extern/glfw/src/cocoa*
        extern/glfw/src/x11*
        extern/glfw/src/wl*
        extern/glfw/src/mir*
        extern/glfw/src/glx*
        extern/glfw/src/linux*
        extern/glfw/src/posix*
    )
elseif (APPLE)
    add_definitions(-D_GLFW_COCOA)
    add_definitions(-D_GLFW_USE_CHDIR)
    add_definitions(-D_GLFW_USE_MENUBAR)
    add_definitions(-D_GLFW_USE_RETINA)

    file(
        GLOB_RECURSE
        DISPOSABLE

        extern/glfw/src/win32*
        extern/glfw/src/x11*
        extern/glfw/src/wl*
        extern/glfw/src/mir*
        extern/glfw/src/glx*
        extern/glfw/src/linux*
    )
endif ()

# Adding recursively every file we want to compile
file(
    GLOB_RECURSE
    SOURCE_FILES
    ${SRC}
)

# Removing unwanted platform-specific files
list(
    REMOVE_ITEM
    SOURCE_FILES
    ${DISPOSABLE}
)

#add_library(ArcV SHARED ${SOURCE_FILES})
add_executable(ArcV ${SOURCE_FILES} examples/main.cpp)

target_link_libraries(
    ArcV ${LIBS}
)
