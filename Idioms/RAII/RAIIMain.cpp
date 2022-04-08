#include "RAII.hpp"

struct Object {
    Object() { std::puts("Object"); }
    ~Object() { std::puts("~Object"); }

    void foo() {}
};

int main() {
    example_1::RAIIFile file("./file.txt");
    std::cout << std::boolalpha << file->is_open() << '\n';

    std::ostringstream ss;
    ss << file->rdbuf();
    std::cout << "File text: " << ss.str() << '\n';

    example_2::RAIIPointer<Object> ptr;
    ptr->foo();
    (*ptr).foo();

    return EXIT_SUCCESS;
}