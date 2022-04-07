cmake_minimum_required(VERSION 3.20.0)

project(LevelUp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

Include(CMakeIncludes/conan.cmake)
Include(CMakeIncludes/warnings_errors.cmake)

add_subdirectory(Concurrency++)
add_subdirectory(MoveSemantics)
add_subdirectory(RAII)