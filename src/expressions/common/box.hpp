//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_COMMON_BOX_HPP__
#define __EXPRESSIONS_COMMON_BOX_HPP__

#include <memory>
#include <type_traits>


namespace expressions {

template<typename T>
class Box {
public:
    Box() requires std::is_default_constructible_v<T> : Box(T {}) {
    }
    Box(T&& obj) : impl_(std::make_unique<T>(std::move(obj))) {
    }
    Box(const T& obj) : impl_(std::make_unique<T>(obj)) {
    }

    Box(const Box& other) : Box(*other.impl_) {
    }
    Box& operator=(const Box& other) {
        if (std::addressof(other) == this) {
            return *this;
        }

        *impl_ = *other.impl_;

        return *this;
    }

    ~Box() = default;

    T& operator*() {
        return *impl_;
    }
    const T& operator*() const {
        return *impl_;
    }

    T* operator->() {
        return impl_.get();
    }
    const T* operator->() const {
        return impl_.get();
    }

private:
    std::unique_ptr<T> impl_;
};

}    // namespace expressions

#endif
