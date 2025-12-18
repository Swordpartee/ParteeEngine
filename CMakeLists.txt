cmake_minimum_required(VERSION 3.23)
project(ParteeEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(include)
include_directories(libs)

# Recursively find all .cpp files in src
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(ParteeEngine ${SOURCES})

# Link libraries
target_link_libraries(ParteeEngine 
    opengl32
    gdi32
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
add_compile_options(/W4 /permissive- /WX)