#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace example_1 {
namespace fs = std::filesystem;

class RAIIFile {
    std::fstream m_file;

  public:
    RAIIFile(fs::path path, std::ios::openmode mode = std::ios::in) {
        if (!fs::exists(path)) {
            std::ofstream file("./file.txt");
            file << "text";
            file.close();
        }

        std::puts("File is openning!");
        m_file.open(path, mode);
    }

    ~RAIIFile() {
        if (m_file.is_open()) {
            std::puts("File is closing!");
            m_file.close();
        }
    }

    std::fstream *operator->() noexcept { return &m_file; }
};
} // namespace example_1

namespace example_2 {
template <typename T> class RAIIPointer {
    T *m_ptr;

  public:
    RAIIPointer() : m_ptr{new T()} {}

    ~RAIIPointer() { delete m_ptr; }

    T *operator->() noexcept { return m_ptr; }
    T& operator*() noexcept { return *m_ptr; }
};
} // namespace example_2