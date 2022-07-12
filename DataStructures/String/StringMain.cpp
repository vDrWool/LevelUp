#include "String.hpp"

int main() {
    // Clear str
    {
        str::string str("asafasfasfasfasfxzvasfasfafasffafssfxzgx");
        
        std::cout << str << std::endl;
        
        str.clear();

        std::cout << str << std::endl;
    }
    // Check copy cnst
    {
        str::string str1("ac");
        str::string str2 = str1;

        std::cout << str1 << " " << str2 << std::endl;

        str1 = str2;

        std::cout << str1 << " " << str2 << std::endl;
    }
    // Check move cnst
    {
        str::string str1("ac");
        str::string str2 = std::move(str1);

        std::cout << std::setw(3) << str1 << " " << str2 << std::endl;

        str1 = std::move(str2);

        std::cout << str1 << " " << str2 << std::endl;
    }
    return EXIT_SUCCESS;
}