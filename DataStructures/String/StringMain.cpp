#include "String.hpp"

int main() {
    str::string str1("ac");
    str::string str2("ca");

    str1.swap(str2);

    std::cout << str1 << " " << str2 << "\n";

    for (auto c : str1) {
        std::cout << c;
    }

    return EXIT_SUCCESS;
}