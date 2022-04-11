#include "CopyAndSwap.hpp"

int* alloc_int_with(int value) { return new int(value); }

void print(const std::string& when, const Object& objA, const Object& objB) {
    std::cout << when << ":\n ";
    std::cout << "\tObjA addr:   " << objA.getpA() << " " << objA.getpB() << std::endl;
    std::cout << "\tObjA values: " << std::setw(16) << objA.getA() << " " << std::setw(16) << objA.getB() << std::endl;
    std::cout << "\tObjB addr:   " << objB.getpA() << " " << objB.getpB() << std::endl;
    std::cout << "\tObjA values: " << std::setw(16) << objB.getA() << " " << std::setw(16) << objB.getB() << std::endl;
}

void sep() { std::cout << std::string(54, '-') << std::endl; }

int main() {
    const std::string before = "Before";
    const std::string after = "After";

    sep();

    Object objA(alloc_int_with(1), alloc_int_with(2));
    Object objB;

    print(before, objA, objB);

    objB = objA;

    print(after, objA, objB);

    sep();

    Object objC(alloc_int_with(3), alloc_int_with(4));
    Object objD;

    print(before, objC, objD);

    objD = std::move(objC);

    print(after, objC, objD);

    sep();

    return EXIT_SUCCESS;
}