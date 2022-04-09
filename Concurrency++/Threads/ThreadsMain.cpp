#include "Thread.hpp"

int main() { 
	sync::example_1::start();
    sync::example_2::start();
    sync::example_3::start();
    sync::example_4::start();

	return EXIT_SUCCESS; 
}