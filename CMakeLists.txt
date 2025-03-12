cmake_minimum_required(VERSION 3.26)
project(Facelib)

set(CMAKE_CXX_STANDARD 17)

# Create the shared library
add_library(FaceLib SHARED FaceLib.cpp)

# Create the executable and link it with the shared library
add_executable(Facelib main.cpp)
target_link_libraries(Facelib FaceLib)