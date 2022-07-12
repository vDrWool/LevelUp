#pragma once
#include <algorithm>
#include <bit>
#include <iostream>
#include <iomanip>
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

    struct long_str_t {
        size_type cap;
        size_type size;
        pointer data;
    };

    constexpr static size_type mc_min_cap = (sizeof(long_str_t) - 1 / sizeof(size_type)) > 2
                                                ? (sizeof(long_str_t) - 1) / sizeof(value_type)
                                                : 2;

    static constexpr size_type mc_bit_mask = std::endian::native == std::endian::big ? 0x01 : 0x60;

    struct short_str_t {
        std::uint8_t size;
        value_type data[mc_min_cap];
    };

    // Just for calculating number of words
    union temp_t {
        long_str_t l;
        short_str_t s;
    };

    constexpr static size_type mc_number_of_words = sizeof(temp_t) / sizeof(size_type);

    struct raw_str_t {
        size_type words[mc_number_of_words];
    };

    struct storage_t {
        union {
            long_str_t l;
            short_str_t s;
            raw_str_t r;
        };
    };

    constexpr static size_type long_size = sizeof(long_str_t);
    constexpr static size_type short_size = sizeof(short_str_t);
    constexpr static size_type raw_size = sizeof(raw_str_t);

    std::pair<storage_t, allocator_type> m_storage;

   private:
    inline long_str_t& get_long() noexcept { return m_storage.first.l; }
    inline short_str_t& get_short() noexcept { return m_storage.first.s; }
    inline raw_str_t& get_raw() noexcept { return m_storage.first.r; }

    inline const long_str_t& read_long() const noexcept { return m_storage.first.l; }
    inline const short_str_t& read_short() const noexcept { return m_storage.first.s; }
    inline const raw_str_t& read_raw() const noexcept { return m_storage.first.r; }

    inline bool is_long() const noexcept { return read_short().size & mc_bit_mask; }

    inline allocator_type& get_alloc() noexcept { return m_storage.second; }

    size_type to_alligned(size_type size) const noexcept {
        constexpr size_type offset = mc_min_cap + 1;

        return (size - size / offset) + offset;
    }

   public:
    constexpr string() noexcept = default;

    string(const_pointer c_str, allocator_type alloc = allocator_type()) {
        if (c_str == nullptr) throw std::runtime_error("c_str cannot be null!");

        const size_type c_str_len = std::strlen(c_str);
        if (c_str_len < mc_min_cap) {
            short_str_t& short_ref = get_short();

            short_ref.size = static_cast<std::uint8_t>(c_str_len);
            std::copy(c_str, c_str + c_str_len, short_ref.data);
        } else {
            long_str_t& long_ref = get_long();

            long_ref.size = c_str_len;
            long_ref.cap = to_alligned(c_str_len);

            long_ref.data = traits_type::allocate(get_alloc(), long_ref.cap);
            if (long_ref.data == nullptr) throw std::bad_alloc();

            std::copy(c_str, c_str + c_str_len, long_ref.data);
        }

        get_alloc() = std::move(alloc);
    }

    string(const string& other) {
        const raw_str_t& raw_other_ref = other.read_raw();

        std::copy(raw_other_ref.words, raw_other_ref.words + mc_number_of_words, get_raw().words);

        if (is_long()) {
            long_str_t& long_ref = get_long();

            long_ref.data = traits_type::allocate(get_alloc(), long_ref.cap);
            if (long_ref.data == nullptr) throw std::bad_alloc();

            const long_str_t& long_other_ref = other.read_long();
            std::copy(long_other_ref.data, long_other_ref.data + long_other_ref.size, long_ref.data);
        }
    }

    string(string&& other) noexcept { swap(other); }

    ~string() {
        if (is_long()) {
            long_str_t& long_ref = get_long();

            traits_type::deallocate(get_alloc(), long_ref.data, long_ref.cap);
        }
    }

   public:
    string& operator=(const string& other) {
        string tmp{other};
        swap(tmp);

        return *this;
    }

    string& operator=(string&& other) noexcept {
        swap(other);

        return *this;
    }

   public:
    void swap(string& other) noexcept { std::swap(get_raw().words, other.get_raw().words); }

    size_type size() const noexcept { return is_long() ? read_long().size : static_cast<size_type>(read_short().size); }

    void clear() noexcept {
        this->~string();

        raw_str_t& raw_ref = get_raw();
        std::fill(raw_ref.words, raw_ref.words + mc_number_of_words, 0);
    }

    bool empty() const noexcept { return size() == 0; }

    iterator begin() const noexcept {
        if (is_long()) {
            return { read_long().data };
        } else {
            return { const_cast<char*>(read_short().data) };  // TODO: how to delete const_cast???
        }
    }
    iterator end() const noexcept { return begin() + size(); }

    friend std::ostream& operator<<(std::ostream& os, const string& str) {
        std::copy(str.begin(), str.end(), std::ostream_iterator<string::value_type>(os, ""));

        return os;
    }

    static constexpr auto npos{static_cast<size_type>(-1)};
};
}  // namespace str