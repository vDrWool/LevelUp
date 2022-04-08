#pragma once
#include <iostream>
#include <vector>

namespace example_1 {
void start() {
    std::vector<int> vec;
    (void)begin(vec); // std::begin // (void) - because std::begin returns iterator with 'nodiscard' attribute.

    std::cout << "out\n";           // operator << is not in global namespace.
    operator<<(std::cout, "out\n"); // same as above.

    // std::cout << endl;
    // std::operator<<(::cout, ::endl); // Error: 'endl' is not declared in this namespace.
    endl(std::cout); // std::endl
}
} // namespace example_1

namespace example_2 {

namespace ns1 {
struct S {};

void foo(S) {}
} // namespace ns1

namespace ns2 {
struct S {};

void foo(S) {}
} // namespace ns2

void start() {
    foo(ns1::S()); // ns1::foo.
    foo(ns2::S()); // ns2::foo.

    // ns1::foo(S()); // Error: 'S' is not declared in this namespace.
    // ns2::foo(S()); // Error: 'S' is not declared in this namespace.
}
} // namespace example_2


namespace ns {
struct S {};
void foo(S) {}
} // namespace ns

void foo(ns::S) {}

void start() 
{ 
    ns::S s;

    // foo(s); // Error: could be ns::foo or foo. 

    // Resolve.
    ns::foo(s);
    ::foo(s); 
    (foo)(s); // Same as ::foo(s).
}
