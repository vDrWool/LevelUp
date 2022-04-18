#pragma once
#include <iostream>
#include <memory>

namespace mem {
template <typename T> class allocator {
  public:
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    [[nodiscard]] constexpr pointer allocate(size_type n) {
        size_type size = n * sizeof(value_type);

        std::cout << "Allocated: " << size << " bytes" << std::endl;

        auto ptr = reinterpret_cast<pointer>(::operator new(size));
        return ptr;
    }

    constexpr void deallocate(pointer p, size_type n) { 
        size_type size = n * sizeof(value_type);

        std::cout << "Deallocated: " << size << " bytes" << std::endl;

        ::operator delete(p, size);
    }
};
} // namespace mem