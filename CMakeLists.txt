cmake_minimum_required(VERSION 3.15)
project(ParteeEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Add compiler flags
target_compile_options(ParteeEngine PRIVATE 
    -g
    -fconcepts
)

# Set output directory
set_target_properties(ParteeEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
