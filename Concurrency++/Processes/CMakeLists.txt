set(TARGET Processes)

set(SOURCE_FILES 
	ProcessesMain.cpp
	Processes.hpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC CONAN_PKG::boost)
