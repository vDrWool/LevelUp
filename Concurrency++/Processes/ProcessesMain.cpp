#include "Processes.hpp"

int main() { 
	example::start_child("ping", {"/n", "25", "127.0.0.1"});


	return EXIT_SUCCESS; 
}
