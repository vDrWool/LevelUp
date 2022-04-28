set(TARGET MemoryManagment)

add_subdirectory(StackAndHeap)
add_subdirectory(SharedPointer)
add_subdirectory(Allocator)

add_library(${TARGET} INTERFACE)