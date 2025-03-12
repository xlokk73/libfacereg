cmake_minimum_required(VERSION 3.30)
project(FaceLib)

set(CMAKE_CXX_STANDARD 20)

add_library(FaceLib SHARED FaceLib.cpp FaceLib.h)
