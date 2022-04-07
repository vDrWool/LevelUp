set(TARGET Concurrency++)

set(SOURCE_FILES 
	ThreadMain.cpp
	Thread.hpp)

add_executable(${TARGET} ${SOURCE_FILES})