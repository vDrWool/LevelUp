#pragma once
#include <atomic>
#include <iostream>
#include <memory>
#include <typeinfo>

namespace example {
struct Obj {
    Obj() { std::puts("Obj"); }
    ~Obj() { std::puts("~Obj"); }
};
} // namespace example

namespace mem {
template <typename T /*, typename D = std::default_delete<T>, typename A = std::allocator<T>*/> struct Control_block {
    using atomic_size_t = std::atomic_size_t;

    // TODO: Come up how to use aligned_storage instead of raw pointer.
    // std::aligned_storage_t<sizeof(T), alignof(T)> _storage;

    T* _storage;
    // D _deleter;
    // A _allocator;
    atomic_size_t _shared_counter;
    atomic_size_t _weak_counter;

    Control_block(T* ptr) noexcept : _shared_counter{1}, _weak_counter{1} { _storage = ptr; }

    void increase_shared(std::uint32_t value = 1) noexcept {
        std::atomic_fetch_add(std::addressof(_shared_counter), value);
    }
    void increase_weak(std::uint32_t value = 1) noexcept {
        std::atomic_fetch_add(std::addressof(_weak_counter), value);
    }

    void decrease_shared(std::uint32_t value = 1) noexcept {
        std::atomic_fetch_sub(std::addressof(_shared_counter), value);
    }
    void decrease_weak(std::uint32_t value = 1) noexcept {
        std::atomic_fetch_sub(std::addressof(_weak_counter), value);
    }

    bool compare_shared_with(std::size_t value) const noexcept {
        return std::atomic_load(std::addressof(_shared_counter)) == value;
    }
    bool compare_weak_with(std::size_t value) const noexcept {
        return std::atomic_load(std::addressof(_weak_counter)) == value;
    }

    std::size_t get_shared(void) const noexcept { return _shared_counter; }
};

template <typename T> class weak_ptr;

template <typename T /*, typename D = std::default_delete<T>, typename A = std::allocator<T>*/> class shared_ptr {
    T* _object;
    Control_block<T>* _ctrl_block;

  public:
    constexpr shared_ptr() noexcept = default;
    shared_ptr(std::nullptr_t) : _object{nullptr}, _ctrl_block{nullptr} {}
    shared_ptr(T* ptr /*, D deleter = D(), A allocator = A()*/)
        : _object{ptr}, _ctrl_block{new Control_block<T>(ptr)} {}

    shared_ptr(const shared_ptr& other) noexcept {
        _object = other._object;
        _ctrl_block = other._ctrl_block;

        _ctrl_block->increase_shared();
    }

    shared_ptr(const weak_ptr<T>& from_weak) noexcept {
        _ctrl_block = from_weak._ctrl_block;
        _object = _ctrl_block->_storage;

        _ctrl_block->increase_shared();
    }

    shared_ptr& operator=(const shared_ptr& other) noexcept {
        _object = other._object;
        _ctrl_block = other._ctrl_block;

        _ctrl_block->increase_shared();

        return *this;
    }

    shared_ptr& operator=(const weak_ptr<T>& from_weak) noexcept {
        _ctrl_block = from_weak._ctrl_block;
        _object = _ctrl_block->_storage;

        _ctrl_block->increase_shared();

        return *this;
    }

    T* operator->() const noexcept { return get(); }
    T& operator*() const noexcept { return *get(); }

    std::size_t use_count() const noexcept { return _ctrl_block->get_shared(); }

    T* get() const noexcept { return _object; }

    ~shared_ptr() {
        _ctrl_block->decrease_shared();

        if (_ctrl_block->compare_shared_with(0)) {
            _ctrl_block->decrease_weak();

            _object->~T();
        }

        if (_ctrl_block->compare_weak_with(0)) {
            ::operator delete(_object);

            delete _ctrl_block;

            _object = nullptr;
            _ctrl_block = nullptr;
        }
    }

    template <typename T> friend class weak_ptr;
    template <typename T, typename... Args> friend shared_ptr<T> make_shared(Args&&... args);
};

template <typename T> class weak_ptr {
    Control_block<T>* _ctrl_block;

  public:
    constexpr weak_ptr() noexcept = default;
    weak_ptr(const weak_ptr& other) noexcept {
        _ctrl_block = other._ctrl_block;

        _ctrl_block->increase_weak();
    }
    weak_ptr(const shared_ptr<T>& other) noexcept {
        _ctrl_block = other._ctrl_block;

        _ctrl_block->increase_weak();
    }
    weak_ptr& operator=(const weak_ptr& other) noexcept {
        _ctrl_block = other._ctrl_block;

        _ctrl_block->increase_weak();

        return *this;
    }
    weak_ptr& operator=(const shared_ptr<T>& other) noexcept {
        _ctrl_block = other._ctrl_block;

        _ctrl_block->increase_weak();

        return *this;
    }

    shared_ptr<T> lock() const { return shared_ptr<T>(*this); }

    bool expired() const noexcept { return _ctrl_block->compare_shared_with(0); }

    std::size_t use_count() const noexcept { return _ctrl_block->get_shared(); }

    ~weak_ptr() {
        _ctrl_block->decrease_weak();

        if (_ctrl_block->compare_weak_with(0) && _ctrl_block->compare_shared_with(0)) {
            ::operator delete(_ctrl_block->_storage);

            delete _ctrl_block;

            _ctrl_block = nullptr;
        }
    }

    friend class shared_ptr<T>;
};

// TODO: make_shared allocates the memory only once!
template <typename T, typename... Args> shared_ptr<T> make_shared(Args&&... args) {
    auto ptr = new T(std::forward<Args>(args)...);
    return shared_ptr<T>(ptr);
}

// template <typename T>
// class enable_shared_from_this {
//     Control_block* _ctrl_block;
//
//     enable_shared_from_this()
// };
} // namespace mem