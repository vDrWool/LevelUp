#include "SharedPointer.hpp"

template <typename T> void print(T& t) { std::cout << typeid(t).name() << " " << t.use_count() << '\n'; }

int main() {
    std::shared_ptr<example::Obj> st{new example::Obj};
    mem::shared_ptr<example::Obj> my{new example::Obj};
    
    print(st);
    print(my);
    
    {
        std::shared_ptr<example::Obj> another_st = st;

        print(st);
        print(another_st);

        mem::shared_ptr<example::Obj> another_my = my;
    
        print(my);
        print(another_my);
    }
    
    print(st);
    print(my);

    return EXIT_SUCCESS;
}