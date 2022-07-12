cmake_minimum_required(VERSION 3.20.0)

project(LevelUp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

message(STATUS "C++ version: " ${CMAKE_CXX_STANDARD})

Include(CMakeIncludes/conan.cmake)
Include(CMakeIncludes/warnings_errors.cmake)

add_subdirectory(Concurrency++)
add_subdirectory(MoveSemantics)
add_subdirectory(ADL)
add_subdirectory(Idioms)
add_subdirectory(RTTI)
add_subdirectory(MemoryManagment)
add_subdirectory(OOP)
add_subdirectory(DataStructures)
add_subdirectory(CopyEllision)