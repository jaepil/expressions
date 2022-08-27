//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_COMMON_ENUMERATE_HPP__
#define __EXPRESSIONS_COMMON_ENUMERATE_HPP__

#include <iterator>
#include <tuple>
#include <type_traits>


namespace expressions {

namespace detail {

struct sentinel_type;
struct no_sentinel_type;

template<typename Tag>
struct enum_value_policy;

template<>
struct enum_value_policy<no_sentinel_type> {
    constexpr explicit enum_value_policy(int32_t start) : value_(start) {
    }

    constexpr int32_t get_value() const noexcept {
        return value_;
    }
    constexpr enum_value_policy& operator++() noexcept {
        ++value_;
        return *this;
    }
    constexpr enum_value_policy operator++(int) noexcept {
        enum_value_policy tmp {*this};
        ++*this;
        return tmp;
    }

    constexpr enum_value_policy& operator--() noexcept {
        --value_;
        return *this;
    }
    constexpr enum_value_policy operator--(int) noexcept {
        enum_value_policy tmp {*this};
        --*this;
        return tmp;
    }

    constexpr enum_value_policy& operator+=(int32_t n) noexcept {
        value_ += n;
        return *this;
    }
    constexpr enum_value_policy operator+(int32_t n) const noexcept {
        enum_value_policy tmp {*this};
        tmp += n;
        return tmp;
    }
    friend constexpr enum_value_policy operator+(
        int32_t n, const enum_value_policy& rhs) noexcept {
        return rhs + n;
    }

    constexpr enum_value_policy& operator-=(int32_t n) noexcept {
        value_ -= n;
        return *this;
    }
    constexpr enum_value_policy operator-(int32_t n) const noexcept {
        enum_value_policy tmp {*this};
        tmp -= n;
        return tmp;
    }
    friend constexpr enum_value_policy operator-(
        int32_t n, const enum_value_policy& rhs) noexcept {
        return rhs - n;
    }

private:
    int32_t value_ = 0;
};

template<>
struct enum_value_policy<sentinel_type> {
    constexpr explicit enum_value_policy(int32_t start) {
        (void)start;
    }

    constexpr int32_t get_value() const noexcept {
        return 0;
    }
    constexpr enum_value_policy& operator++() noexcept {
        return *this;
    }
    constexpr enum_value_policy operator++(int) noexcept {
        enum_value_policy tmp {*this};
        return tmp;
    }

    constexpr enum_value_policy& operator--() noexcept {
        return *this;
    }
    constexpr enum_value_policy operator--(int) noexcept {
        enum_value_policy tmp {*this};
        return tmp;
    }

    constexpr enum_value_policy& operator+=(int32_t n) noexcept {
        (void)n;
        return *this;
    }
    constexpr enum_value_policy operator+(int32_t n) const noexcept {
        enum_value_policy tmp {*this};
        tmp += n;
        return tmp;
    }
    friend constexpr enum_value_policy operator+(
        int32_t n, const enum_value_policy& rhs) noexcept {
        return rhs + n;
    }

    constexpr enum_value_policy& operator-=(int32_t n) noexcept {
        (void)n;
        return *this;
    }
    constexpr enum_value_policy operator-(int32_t n) const noexcept {
        enum_value_policy tmp {*this};
        tmp -= n;
        return tmp;
    }
    friend constexpr enum_value_policy operator-(
        int32_t n, const enum_value_policy& rhs) noexcept {
        return rhs - n;
    }
};

template<typename Iterator, typename Sentinel = no_sentinel_type>
class lazy_enumeration_iterator {
public:
    using iterator_traits = std::iterator_traits<std::remove_cvref_t<Iterator>>;
    using iterator_category = typename iterator_traits::iterator_category;
    using difference_type = typename iterator_traits::difference_type;
    using value_type
        = std::tuple<int32_t, typename iterator_traits::value_type>;
    using pointer = std::tuple<int32_t, typename iterator_traits::pointer>;
    using reference
        = std::tuple<int32_t, const typename iterator_traits::value_type&>;

    constexpr lazy_enumeration_iterator() = default;
    constexpr explicit lazy_enumeration_iterator(Iterator it)
        : it_(it), current_(0) {
    }
    constexpr lazy_enumeration_iterator(Iterator it, int32_t start)
        : it_(it), current_(start) {
    }

    constexpr operator lazy_enumeration_iterator<const Iterator, Sentinel>()
        const noexcept {
        return {it_, current_};
    }

    constexpr reference operator*() const noexcept {
        return std::make_tuple(current_.get_value(), std::cref(*it_));
    }
    constexpr pointer operator->() const noexcept {
        return std::make_tuple(current_.get_value(), std::addressof(*it_));
    }

    constexpr lazy_enumeration_iterator& operator++() noexcept {
        ++current_;
        ++it_;
        return *this;
    }
    constexpr lazy_enumeration_iterator operator++(int) noexcept {
        lazy_enumeration_iterator tmp {*this};
        ++*this;
        return tmp;
    }

    constexpr lazy_enumeration_iterator& operator--() noexcept {
        --current_;
        --it_;
        return *this;
    }
    constexpr lazy_enumeration_iterator operator--(int) noexcept {
        lazy_enumeration_iterator tmp {*this};
        --*this;
        return tmp;
    }

    constexpr lazy_enumeration_iterator& operator+=(
        difference_type n) noexcept {
        current_ += n;
        std::advance(it_, n);
        return *this;
    }
    constexpr lazy_enumeration_iterator operator+(
        difference_type n) const noexcept {
        lazy_enumeration_iterator tmp {*this};
        tmp += n;
        return tmp;
    }
    friend constexpr lazy_enumeration_iterator operator+(
        difference_type n, const lazy_enumeration_iterator& rhs) noexcept {
        return rhs + n;
    }

    constexpr lazy_enumeration_iterator& operator-=(
        difference_type n) noexcept {
        current_ -= n;
        std::advance(it_, -n);
        return *this;
    }
    constexpr lazy_enumeration_iterator operator-(
        difference_type n) const noexcept {
        lazy_enumeration_iterator tmp {*this};
        tmp -= n;
        return tmp;
    }
    friend constexpr lazy_enumeration_iterator operator-(
        difference_type n, const lazy_enumeration_iterator& rhs) noexcept {
        return rhs - n;
    }

    template<typename I, typename S>
    constexpr bool operator==(
        const lazy_enumeration_iterator<I, S>& rhs) const noexcept {
        return it_ == rhs.it_;
    }
    template<typename I, typename S>
    constexpr bool operator!=(
        const lazy_enumeration_iterator<I, S>& rhs) const noexcept {
        return !(*this == rhs);
    }

    constexpr bool operator==(const Iterator& rhs) const noexcept {
        return it_ == rhs;
    }
    constexpr bool operator!=(const Iterator& rhs) const noexcept {
        return !(*this == rhs);
    }

private:
    template<typename I, typename S>
    friend class lazy_enumeration_iterator;

    Iterator it_ {};    // NOLINT
    enum_value_policy<Sentinel> current_ {};
};

template<typename Iterator>
class enumeration_wrapper {
public:
    using iterator_traits = std::iterator_traits<std::remove_cvref_t<Iterator>>;
    using reference = typename iterator_traits::reference;
    using const_reference = const reference;
    using iterator = lazy_enumeration_iterator<Iterator>;
    using const_iterator = lazy_enumeration_iterator<const Iterator>;
    using size_type = size_t;
    using difference_type = typename iterator_traits::difference_type;
    using value_type = typename iterator_traits::value_type;
    using pointer = typename iterator_traits::pointer;
    using const_pointer = const pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr enumeration_wrapper(Iterator begin, Iterator end, int32_t start)
        : begin_(begin), end_(end), start_(start) {
    }

    constexpr auto begin() const noexcept {
        return lazy_enumeration_iterator {begin_, start_};
    }
    constexpr auto end() const noexcept {
        return lazy_enumeration_iterator<Iterator, sentinel_type> {end_};
    }

    constexpr bool empty() const noexcept {
        return size() == 0;
    }

    constexpr size_type size() const noexcept {
        return std::distance(end_, begin_);
    }

private:
    Iterator begin_ {};
    Iterator end_ {};
    int32_t start_ = 0;
};
}    // namespace detail

template<
    typename T,
    std::enable_if_t<
        std::is_base_of_v<std::input_iterator_tag,
                          typename std::iterator_traits<T>::iterator_category>,
        int> = 0>
auto enumerate(T begin, T end, int32_t start = 0) {
    return detail::enumeration_wrapper {begin, end, start};
}

template<typename T,
         std::enable_if_t<
             std::is_base_of_v<
                 std::input_iterator_tag,
                 typename std::iterator_traits<
                     decltype(std::declval<T>().begin())>::iterator_category>,
             int> = 0>
auto enumerate(T&& container, int32_t start = 0) {
    return enumerate(std::begin(container), std::end(container), start);
}

template<typename T, size_t N>
auto enumerate(T (&array)[N], int32_t start = 0) {
    return enumerate(std::begin(array), std::end(array), start);
}

template<typename T, size_t N>
auto enumerate(const std::array<T, N>& array, int32_t start = 0) {
    return enumerate(std::begin(array), std::end(array), start);
}

}    // namespace expressions

#endif
