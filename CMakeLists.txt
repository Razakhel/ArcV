cmake_minimum_required(VERSION 3.6)
project(ArcV)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-value -Wno-unused-parameter -Wno-unused-function")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif ()

option(ARCV_BUILD_STATIC "Build ArcV statically" ON)
option(ARCV_BUILD_EXAMPLES "Build examples along ArcV" ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/extern
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/png
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glew/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

AUX_SOURCE_DIRECTORY(extern/png PNG_SRC)
AUX_SOURCE_DIRECTORY(extern/glew/src GLEW_SRC)
AUX_SOURCE_DIRECTORY(extern/glfw/src GLFW_SRC)

set(
    SRC

    ${PNG_SRC}
    ${GLEW_SRC}
    ${GLFW_SRC}

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
    add_definitions(
        -D_GLFW_X11
        -D_GLFW_HAS_XF86VM

        #-fsanitize=undefined
        #-fsanitize=address
        #-fsanitize=leak
    )

    file(
        GLOB
        DISPOSABLE

        extern/glfw/src/cocoa*
        extern/glfw/src/win32*
        extern/glfw/src/wl*
        extern/glfw/src/wgl*
        extern/glfw/src/mir*
        extern/glfw/src/*.m
    )

    set(
        LIBS

        #asan
        #ubsan
    )

    set(
        DEPS

        X11
        GL
        Xrandr
        Xcursor
        Xinerama
        Xxf86vm
        pthread
        dl

        v4l2
    )
elseif (WIN32)
    add_definitions(
        -D_GLFW_WIN32
        -DGLEW_BUILD
        #-DGLEW_NO_GLU
    )

    file(
        GLOB
        DISPOSABLE

        extern/glfw/src/cocoa*
        extern/glfw/src/x11*
        extern/glfw/src/wl*
        extern/glfw/src/mir*
        extern/glfw/src/glx*
        extern/glfw/src/linux*
        extern/glfw/src/posix*
        extern/glfw/src/*.m
    )

    set(
        DEPS

        opengl32
    )
elseif (APPLE)
    add_definitions(
        -D_GLFW_COCOA
        -D_GLFW_USE_CHDIR
        -D_GLFW_USE_MENUBAR
        -D_GLFW_USE_RETINA
    )

    file(
        GLOB
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
    GLOB
    SOURCE_FILES

    ${SRC}
)

# Removing unwanted platform-specific files
list(
    REMOVE_ITEM
    SOURCE_FILES

    ${DISPOSABLE}
)

if (${ARCV_BUILD_STATIC})
    add_library(ArcV STATIC ${SOURCE_FILES})
else ()
    add_library(ArcV SHARED ${SOURCE_FILES})
endif ()

target_link_libraries(
    ArcV

    ${LIBS}
    ${DEPS}
)

if (${ARCV_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif ()
