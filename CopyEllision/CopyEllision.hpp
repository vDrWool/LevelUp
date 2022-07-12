#pragma once

#include <iostream>

namespace example {
struct Obj {
    Obj() { std::puts("Obj"); }
    Obj(const Obj&) { std::puts("const Obj&"); }
    Obj(Obj&&) noexcept { std::puts("Obj&&"); }
    ~Obj() { std::puts("~Obj"); }
};

Obj RVO() { return Obj(); }

Obj NRVO() {
    Obj obj;
    return obj;
}

Obj not_a_copy_ellision(bool condition) {
    Obj obj;
    if (condition) {
        return obj;
    } else {
        return Obj();
    }
}
}  // namespace example