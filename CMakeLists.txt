cmake_minimum_required(VERSION 3.10)
project(FaceRecognition)

set(CMAKE_CXX_STANDARD 17)

# Set the OpenCV_DIR to point to your specific OpenCV installation
set(OpenCV_DIR "C:/Users/vassg031/CLionProjects/FaceRecognition/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/lib")

# Find OpenCV package
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV_VERSION: ${OpenCV_VERSION}")
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Create FaceLib library
add_library(FaceLib SHARED
        FaceLib.cpp
        FaceLib.h
)

# Add compiler definition when building the library
target_compile_definitions(FaceLib PRIVATE FACELIB_EXPORTS)

# Link OpenCV to FaceLib
target_link_libraries(FaceLib ${OpenCV_LIBS})

# Create main executable
add_executable(Facelib main.cpp)

# Link to FaceLib
target_link_libraries(Facelib FaceLib)

# If needed for filesystem functionality
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(Facelib stdc++fs)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)