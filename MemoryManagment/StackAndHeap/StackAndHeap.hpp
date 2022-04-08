#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace example_1 {
void stack() {
    // The allocation happens on contiguous blocks of memory.
    // It calls a stack memory allocation because the allocation happens in the function call stack.
    // The size of memory to be allocated is known to the compiler and whenever a function is called,
    // its variables get memory allocated on the stack.
    // And whenever the function call is over, the memory for the variables is de-allocated.

    int a = 0;
    short b[16];
    const int c = 20;
    char d[c];

    static_cast<void>(a);  // Disable compiler warnings.
    static_cast<void>(b);  // Disable compiler warnings.
    static_cast<void>(c);  // Disable compiler warnings.
    static_cast<void>(d);  // Disable compiler warnings.

    // but not:
    // const int c = a;
    // char d[c]; // Error: expression must be a constant value.
    // For more safe use constexpr:
    // constexpr int c = a; // CE
    // constexpr int c = 20; // Ok
    // char d[c];
}

void heap() {
    // The memory is allocated during the execution of instructions written by programmers.
    // It is called heap because it is a pile of memory space available to programmers to allocated and de-allocate.
    // Every time when we made an object it always creates in Heap-space and
    // the referencing information to these objects are always stored in Stack-memory.
    // Heap memory allocation isn’t as safe as Stack memory allocation was because
    // the data stored in this space is accessible or visible to all threads.
    // If a programmer does not handle this memory well, a memory leak can happen in the program.

    int *a = new int;           // allocated but not initialized.
    int *b = new int(0);        // allocated and initialized.
    int *c = new int[2];        // allocated contiguous blocks of memory on the heap but not initialized.
    int *d = new int[2]{1, 2};  // allocated contiguous blocks of memory on the heap and initialized.

    // You must always delete all allocated memory using delete or delete[].
    delete a;
    delete b;
    delete[] c;
    delete[] d;

    struct Obj {
        ~Obj() = default;
    };

    void *mem = std::malloc(sizeof(Obj));  // allocated.
    Obj *obj = new (mem) Obj();            // allocated, created and initialized: Obj() same as Obj a = data or Obj a{}.

    obj->~Obj();  // destroyed.
    delete obj;   // de-allocated.
}

void start() {
    stack();
    heap();
}
}  // namespace example_1

namespace example_2 {
void string() {
    const char *c_str = "c_str";  // C string literal. Stores string on the stack.
    std::string str = "str";      // C++ string. Could store string on the heap or on the stack
                                  // (if string is smaller than _BUF_SIZE - SSO).
    static_cast<void>(c_str);

    // Trivial implimintation of string has fields:
    // std::size_t size;      // size of the string.
    // std::size_t capacity;  // how much string can store.
    // char* data;            // pointer to the data.
    // size, capacity and pointer stored at the stack.
    // But the data which is indicated by the pointer stored at the heap.
    // ------------         ---------
    // |   stack  |         | heap  |
    // |   size   |         |       |
    // | capacity |         |       |
    // |  data*   |   -->   |  's'  |
    // |          |         |  't'  |
    // |          |         |  'r'  |
    // ------------         ---------
}

void arr() {
    int arr[3]{1, 2, 3};            // Stores data on the stack.
    std::vector<int> vec{1, 2, 3};  // Stores data on the heap.
    // Same as for std::string.
    // Trivial implimintation of vector has fields:
    // std::size_t size;      // size of the string.
    // std::size_t capacity;  // how much string can store.
    // T* data;               // pointer to the data.
    // size, capacity and pointer stored at the stack.
    // But the data which is indicated by the pointer stored at the heap.
    // ------------         ---------
    // |   stack  |         | heap  |
    // |   size   |         |       |
    // | capacity |         |       |
    // |  data*   |   -->   |   1   |
    // |          |         |   2   |
    // |          |         |   3   |
    // ------------         ---------
    // For std::array:
    // std::size_t size;
    // T* data;
}

void start() {
    string();
    arr();
}
}  // namespace example_2