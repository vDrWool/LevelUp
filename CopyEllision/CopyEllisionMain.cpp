#include "CopyEllision.hpp"

int main() { 
	example::Obj a = example::RVO();
    example::Obj b = example::NRVO();
    example::Obj c = example::not_a_copy_ellision(true);


	return EXIT_SUCCESS;
}