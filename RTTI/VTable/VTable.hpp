#pragma once

#include <functional>
#include <iostream>

// Trivial implimentation of the VTable.
namespace example_1 {
using fooPtr = void (*)();
using barPtr = int (*)();

struct VTable {
    fooPtr foo;
    barPtr bar;
};

struct Base {
    Base() {
        vtable.foo = nullptr;  // Pure virtual method
        vtable.bar = []() { return 1; };

        std::puts("Base");
    }

    void foo() { return vtable.foo(); };
    int bar() { return vtable.bar(); }

    virtual ~Base() { std::puts("~Base"); };

   protected:
    VTable vtable;
};

struct Derived : Base {
    Derived() {
        vtable.foo = []() {};
        vtable.bar = Base::vtable.bar;

        std::puts("Derived");
    }

    void foo() { return vtable.foo(); }

    ~Derived() { std::puts("~Derived"); }
};

void start() {
    Base* b = new Derived();

    b->foo();
    b->bar();

    delete b;
}
}  // namespace example_1
