cmake_minimum_required(VERSION 3.10)
project(Moleculer_orbitals)

set(CMAKE_CXX_STANDARD 17)

# Include GLFW and GLAD
add_subdirectory(third_party/glfw)

include_directories(
    third_party/glad/include
    third_party/glfw/include
    src
)

# Source files
set(SOURCES
    src/main.cpp
    src/shader.h
    third_party/glad/src/glad.c
    src/shader.cpp
)

set(RESOURCE_DIRS shaders)
foreach(dir IN LISTS RESOURCE_DIRS)
    file(COPY ${CMAKE_SOURCE_DIR}/${dir} DESTINATION ${CMAKE_BINARY_DIR})
endforeach()


add_executable(Moleculer_orbitals ${SOURCES})

# Link libraries
target_link_libraries(Moleculer_orbitals glfw opengl32)
