#include "SharedPointer.hpp"

template <typename T> void print(T& t) { std::cout << typeid(t).name() << " " << t.use_count() << '\n'; }

int main() {
    std::weak_ptr<example::Obj> st_g_weak;
    mem::weak_ptr<example::Obj> my_g_weak;

    {
        auto st = std::make_shared<example::Obj>();
        auto my = mem::make_shared<example::Obj>();

        print(st);
        print(my);

        st_g_weak = st;
        my_g_weak = my;

        {
            std::weak_ptr<example::Obj> st_weak = st;
            mem::weak_ptr<example::Obj> my_weak = my;

            print(st);
            print(my);

            auto st_from_weak = st_weak.lock();
            auto my_from_weak = my_weak.lock();

            print(st);
            print(my);
        }

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
    }

    return EXIT_SUCCESS;
}