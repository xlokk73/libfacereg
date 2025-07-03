cmake_minimum_required(VERSION 3.12)
project(FaceRecognition)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "C:/Users/vassg031/CLionProjects/FaceRecognition/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/lib")

# Find the OpenCV package. This is required for the project to build.
find_package(OpenCV REQUIRED)

# Log OpenCV details for debugging purposes.
message(STATUS "OpenCV found: ${OpenCV_VERSION}")
message(STATUS "Using OpenCV libraries: ${OpenCV_LIBS}")
message(STATUS "Using OpenCV includes: ${OpenCV_INCLUDE_DIRS}")

# Define the shared library target "FaceLib".
add_library(FaceLib SHARED
        FaceLib.cpp
        FaceLib.h
)

# Define FACELIB_EXPORTS when compiling the FaceLib library itself.
# This is used by FaceLib.h to set the correct dllexport/dllimport attributes.
target_compile_definitions(FaceLib PRIVATE FACELIB_EXPORTS)

# Link FaceLib to the required OpenCV headers and libraries.
# PUBLIC propagates these properties to targets that link against FaceLib.
target_include_directories(FaceLib PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(FaceLib PUBLIC ${OpenCV_LIBS})

# Define the main executable for the application.
add_executable(FaceRecognitionApp main.cpp)

# Link the executable against our FaceLib library.
target_link_libraries(FaceRecognitionApp PRIVATE FaceLib)

# Add stdc++fs for older GCC compilers that require it for <filesystem>.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(FaceRecognitionApp PRIVATE stdc++fs)
endif()

# Set standardized output directories for executables and libraries.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)