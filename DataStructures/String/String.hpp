#pragma once
#include <algorithm>
#include <bit>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

namespace str {
class string {
    using value_type = char;
    using allocator_type = std::allocator<value_type>;
    using traits_type = std::allocator_traits<allocator_type>;
    using size_type = traits_type::size_type;
    using difference_type = traits_type::difference_type;
    using pointer = traits_type::pointer;
    using const_pointer = traits_type::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;

    class string_iterator {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = string::difference_type;
        using value_type = string::value_type;
        using pointer = string::pointer;
        using const_pointer = string::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;

        pointer ptr;

       public:
        string_iterator() = default;
        string_iterator(pointer ptr) : ptr{ptr} {}

        reference operator*() const noexcept { return *ptr; }
        pointer operator->() const noexcept { return ptr; }

        string_iterator& operator++() noexcept {
            ptr++;
            return *this;
        }
        string_iterator& operator++(int) noexcept {
            string_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        string_iterator& operator--() noexcept {
            ptr--;
            return *this;
        }
        string_iterator& operator--(int) noexcept {
            string_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        string_iterator operator+(std::ptrdiff_t value) noexcept { return ptr + value; }

        string_iterator operator-(std::ptrdiff_t value) noexcept { return ptr - value; }

        friend bool operator==(const string_iterator& l, const string_iterator& r) { return l.ptr == r.ptr; };
        friend bool operator!=(const string_iterator& l, const string_iterator& r) { return l.ptr != r.ptr; };
    };

    using iterator = string_iterator;
    using const_iterator = const string_iterator;
    // using reverse_iterator       = ;
    // using const_reverse_iterator = ;

    static constexpr size_type __short_mask = std::endian::native == std::endian::big ? 0x01 : 0x80;
    static constexpr size_type __long_mask = std::endian::native == std::endian::big ? 0x1ul : ~(size_type(~0) >> 1);

    struct __long {
        size_type cap;
        size_type size;
        pointer data;
    };

    constexpr static size_type __min_cap = (sizeof(__long) - 1 / sizeof(size_type)) > 2
                                               ? (sizeof(__long) - 1) / sizeof(value_type)
                                               : 2;

    struct __short {
        std::uint8_t size;
        value_type data[__min_cap];
    };

    // Just for calculating __n_words
    union __ulx {
        __long lx;
        __short sx;
    };

    constexpr static size_type __n_words = sizeof(__ulx) / sizeof(size_type);

    struct __raw {
        size_type words[__n_words];
    };

    struct __storage {
        union {
            __long l;
            __short s;
            __raw r;
        };
    };

    std::pair<__storage, allocator_type> storage;

   private:
    inline __long& get_long() noexcept { return storage.first.l; }
    inline __short& get_short() noexcept { return storage.first.s; }
    inline __raw& get_raw() noexcept { return storage.first.r; }

    inline const __long& read_long() const noexcept { return storage.first.l; }
    inline const __short& read_short() const noexcept { return storage.first.s; }
    inline const __raw& read_raw() const noexcept { return storage.first.r; }

    inline bool is_long() const noexcept { return read_short().size & __short_mask; }

    inline allocator_type& get_alloc() noexcept { return storage.second; }

    size_type to_alligned(size_type size) const noexcept {
        constexpr size_type __offset = __min_cap + 1;

        return (size - size / __offset) + __offset;
    }

    // void set_short_pointer(const_pointer pointer) { traits_type:: get_short().data; }

   public:
    constexpr string() noexcept = default;

    string(const_pointer c_str, /*const*/ allocator_type /*&*/ alloc = allocator_type()) {
        if (c_str == nullptr) throw std::runtime_error("c_str is empty!");

        const size_type c_str_len = std::strlen(c_str);
        if (c_str_len < __min_cap) {
            __short& short_ref = get_short();

            short_ref.size = static_cast<std::uint8_t>(c_str_len);
            std::copy(c_str, c_str + c_str_len, short_ref.data);
        } else {
            __long& long_ref = get_long();

            long_ref.size = c_str_len;
            long_ref.cap = to_alligned(c_str_len);

            long_ref.data = traits_type::allocate(get_alloc(), long_ref.cap);
            std::copy(c_str, c_str + c_str_len, long_ref.data);
        }

        get_alloc() = std::move(alloc);
    }

    ~string() {
        if (is_long()) {
            __long& long_ref = get_long();

            traits_type::deallocate(get_alloc(), long_ref.data, long_ref.cap);
        }
    }

   public:
   public:
    void swap(string& other) noexcept { std::swap(get_raw().words, other.get_raw().words); }

    size_type size() const noexcept { return is_long() ? read_long().size : static_cast<size_type>(read_short().size); }
    iterator begin() const noexcept {
        if (is_long()) {
            return read_long().data;
        } else {
            return const_cast<char*>(read_short().data);  // TODO: how to delete const_cast???
        }
    }
    iterator end() const noexcept {
        return begin() + size();
    }

    friend std::ostream& operator<<(std::ostream& os, const string& str) {
        if (str.is_long()) {
            const __long& long_ref = str.read_long();

            std::copy(long_ref.data, long_ref.data + long_ref.size, std::ostream_iterator<string::value_type>(os, ""));
        } else {
            const __short& short_ref = str.read_short();

            std::copy(short_ref.data, short_ref.data + short_ref.size,
                      std::ostream_iterator<string::value_type>(os, ""));
        }

        return os;
    }

    static constexpr auto npos{static_cast<size_type>(-1)};
};
}  // namespace str