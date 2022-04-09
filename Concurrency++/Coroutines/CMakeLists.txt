set(TARGET Coroutines)

set(SOURCE_FILES 
	CoroutinesMain.cpp
	Coroutines.hpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC CONAN_PKG::andreasbuhr-cppcoro)
