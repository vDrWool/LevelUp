#pragma once

#include <iostream>

namespace bad_example {
class Object {
   public:
    std::int32_t a_ = 1;
    std::int32_t b_ = 1;

    void work() {
        if (a_ < 0 || b_ < 0 || a_ > 5 || b_ > 0) throw std::exception("Critical error!");

        std::cout << a_ + b_ << " OK\n";
    }
};
}  // namespace bad_example

namespace good_example {
class Object {
    std::int32_t a_ = 1;
    std::int32_t b_ = 1;

   public:
    void set_a(std::int32_t a) {
        if (a < 0 || a > 5) throw std::runtime_error("Field 'a' must be 0 < a < 5!");

        a_ = a;
    }

    void set_b(std::int32_t b) {
        if (b < 0 || b > 5) throw std::runtime_error("Field 'b' must be 0 < a < 5!");

        b_ = b;
    }

    void work() { std::cout << a_ + b_ << " OK\n"; }
};
}  // namespace good_example

void start() {
    bad_example::Object bad;
    try {
        // User decided to changes object fields.
        bad.a_ = 0;
        bad.b_ = 10;

        bad.work();

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    good_example::Object good;
    try {
        good.set_a(2);
        good.set_b(10);

        good.work();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << "Fields sets as a = 1 and b = 2\n";

        good.set_a(1);
        good.set_b(2);

        good.work();
    }
}