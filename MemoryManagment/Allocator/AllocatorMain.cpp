#include "Allocator.hpp"

int main() {
    using AllocTraitsInt = std::allocator_traits<mem::allocator<int>>;

    mem::allocator<int> alloc;

    int* ptr = AllocTraitsInt::allocate(alloc, sizeof(int));
    AllocTraitsInt::construct(alloc, ptr, 1);

    std::cout << "Pointer = " << *ptr << std::endl;

    AllocTraitsInt::destroy(alloc, ptr);
    AllocTraitsInt::deallocate(alloc, ptr, sizeof(ptr));

    return EXIT_SUCCESS;
}