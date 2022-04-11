#pragma once

#include <algorithm>
#include <iostream>
#include <iomanip>

class Object {
    std::int32_t* field_a;
    std::int32_t* field_b;

  public:
    Object(std::int32_t* p1 = nullptr, std::int32_t* p2 = nullptr) : field_a{p1}, field_b{p2} {}

    Object(const Object& obj) : field_a{new std::int32_t(*obj.field_a)}, field_b{new std::int32_t(*obj.field_b)} {}
    Object& operator=(const Object& obj) {
        Object copy(obj);
        swap(*this, copy);

        return *this;
    }

    Object(Object&& obj) noexcept : Object() { swap(*this, obj); }
    Object& operator=(Object&& obj) noexcept {
        swap(*this, obj);
        return *this;
    }

    ~Object() {
        delete field_a;
        delete field_b;
    }

    std::int32_t* getpA() const { return field_a; }
    std::int32_t* getpB() const { return field_b; }

    std::int32_t getA() const {
        if (field_a)
            return *field_a;
        else
            return std::numeric_limits<std::int32_t>::min();
    }
    std::int32_t getB() const {
        if (field_b)
            return *field_b;
        else
            return std::numeric_limits<std::int32_t>::min();
    }

    friend void swap(Object& left, Object& right) noexcept {
        using std::swap;
        swap(left.field_a, right.field_a);
        swap(left.field_b, right.field_b);
    }
};