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

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# Create executable
add_executable(ParteeEngine ${SOURCES})

# Link libraries
target_link_libraries(ParteeEngine 
    opengl32
    gdi32
)

target_compile_options(ParteeEngine PRIVATE /W4 /permissive- /WX)
target_compile_options(ParteeEngine PRIVATE "$<$<CONFIG:Debug>:/Zi>")