//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_SUPPORT_BOOST_VARIANT_HPP__
#define __EXPRESSIONS_SUPPORT_BOOST_VARIANT_HPP__

#include <boost/mp11.hpp>
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/variant.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <concepts>


namespace expressions::support::boost {

namespace x3 = ::boost::spirit::x3;

namespace detail {
template<typename T>
struct remove_forward_ast {
    using type = T;
};

template<typename T>
struct remove_forward_ast<x3::forward_ast<T>> {
    using type = T;
};

template<typename T>
using remove_forward_ast_t = typename remove_forward_ast<T>::type;

template<typename T>
struct HoldsAlternativeVisitor : ::boost::static_visitor<bool> {
    HoldsAlternativeVisitor() = default;

    bool operator()(const T&) const {
        return true;
    }
    bool operator()(const auto&) const {
        return false;
    }
};
}    // namespace detail

template<typename T, template<typename...> typename VariantType, typename... Ts,
         std::enable_if_t<std::same_as<VariantType<Ts...>, x3::variant<Ts...>>,
                          int> = 0>
constexpr bool holds_alternative(const VariantType<Ts...>& value) {
    using underlying_type
        = detail::remove_forward_ast_t<std::remove_cvref_t<T>>;

    if constexpr (std::is_arithmetic_v<T>) {
        return value.apply_visitor(
            detail::HoldsAlternativeVisitor<underlying_type> {});
    } else if constexpr (::boost::mp11::mp_contains<
                             typename VariantType<Ts...>::variant_type::types,
                             underlying_type>::value) {
        return value.apply_visitor(
            detail::HoldsAlternativeVisitor<underlying_type> {});
    } else {
        return value.apply_visitor(detail::HoldsAlternativeVisitor<
                                   x3::forward_ast<underlying_type>> {});
    }
}

template<
    typename T, template<typename...> typename VariantType, typename... Ts,
    std::enable_if_t<std::same_as<VariantType<Ts...>, ::boost::variant<Ts...>>,
                     int> = 0>
constexpr bool holds_alternative(const VariantType<Ts...>& value) {
    using underlying_type
        = detail::remove_forward_ast_t<std::remove_cvref_t<T>>;

    return ::boost::apply_visitor(
        detail::HoldsAlternativeVisitor<underlying_type> {}, value);
}

template<typename... Ts, typename VariantType,
         std::enable_if_t<(sizeof...(Ts) > 0), int> = 0>
constexpr bool holds_any_of(const VariantType& value) {
    return (... || holds_alternative<Ts>(value));
}

template<typename T, typename... Ts, template<typename...> typename VariantType,
         std::enable_if_t<std::same_as<VariantType<Ts...>, x3::variant<Ts...>>,
                          int> = 0>
constexpr auto get(const VariantType<Ts...>& value) -> decltype(auto) {
    using underlying_type
        = detail::remove_forward_ast_t<std::remove_cvref_t<T>>;

    if constexpr (std::is_arithmetic_v<T>) {
        return ::boost::get<underlying_type>(value);
    } else if constexpr (::boost::mp11::mp_contains<
                             typename VariantType<Ts...>::variant_type::types,
                             underlying_type>::value) {
        return ::boost::get<underlying_type>(value);
    } else {
        return ::boost::get<x3::forward_ast<underlying_type>>(value).get();
    }
}

template<
    typename T, typename... Ts, template<typename...> typename VariantType,
    std::enable_if_t<std::same_as<VariantType<Ts...>, ::boost::variant<Ts...>>,
                     int> = 0>
constexpr auto get(const VariantType<Ts...>& value) -> decltype(auto) {
    using underlying_type
        = detail::remove_forward_ast_t<std::remove_cvref_t<T>>;

    return ::boost::get<underlying_type>(value);
}

template<typename T, typename... Ts, template<typename...> typename VariantType,
         std::enable_if_t<std::same_as<VariantType<Ts...>, x3::variant<Ts...>>,
                          int> = 0>
constexpr auto get_if(VariantType<Ts...>* value) -> decltype(auto) {
    using underlying_type
        = detail::remove_forward_ast_t<std::remove_cvref_t<T>>;

    if (!holds_alternative<T>(*value)) {
        return static_cast<underlying_type*>(nullptr);
    }

    if constexpr (std::is_arithmetic_v<T>) {
        return ::boost::get<underlying_type>(value);
    } else if constexpr (::boost::mp11::mp_contains<
                             typename VariantType<Ts...>::variant_type::types,
                             underlying_type>::value) {
        return ::boost::get<underlying_type>(value);
    } else {
        return ::boost::get<x3::forward_ast<underlying_type>>(value)
            ->get_pointer();
    }
}

template<typename T, typename... Ts, template<typename...> typename VariantType,
         std::enable_if_t<std::same_as<VariantType<Ts...>, x3::variant<Ts...>>,
                          int> = 0>
constexpr auto get_if(const VariantType<Ts...>* value) -> decltype(auto) {
    using underlying_type
        = detail::remove_forward_ast_t<std::remove_cvref_t<T>>;

    if (!holds_alternative<T>(*value)) {
        return static_cast<const underlying_type*>(nullptr);
    }

    if constexpr (std::is_arithmetic_v<T>) {
        return ::boost::get<underlying_type>(value);
    } else if constexpr (::boost::mp11::mp_contains<
                             typename VariantType<Ts...>::variant_type::types,
                             underlying_type>::value) {
        return ::boost::get<underlying_type>(value);
    } else {
        return ::boost::get<x3::forward_ast<underlying_type>>(value)
            ->get_pointer();
    }
}

template<
    typename T, typename... Ts, template<typename...> typename VariantType,
    std::enable_if_t<std::same_as<VariantType<Ts...>, ::boost::variant<Ts...>>,
                     int> = 0>
constexpr auto get_if(VariantType<Ts...>* value) -> decltype(auto) {
    using underlying_type
        = detail::remove_forward_ast_t<std::remove_cvref_t<T>>;

    if (!holds_alternative<T>(*value)) {
        return static_cast<underlying_type*>(nullptr);
    }

    return ::boost::get<underlying_type>(value);
}

template<
    typename T, typename... Ts, template<typename...> typename VariantType,
    std::enable_if_t<std::same_as<VariantType<Ts...>, ::boost::variant<Ts...>>,
                     int> = 0>
constexpr auto get_if(const VariantType<Ts...>* value) -> decltype(auto) {
    using underlying_type
        = detail::remove_forward_ast_t<std::remove_cvref_t<T>>;

    if (!holds_alternative<T>(*value)) {
        return static_cast<const underlying_type*>(nullptr);
    }

    return ::boost::get<const underlying_type>(value);
}

}    // namespace expressions::support::boost

namespace expressions::support {

using ::expressions::support::boost::holds_alternative;
using ::expressions::support::boost::holds_any_of;

using ::expressions::support::boost::get;
using ::expressions::support::boost::get_if;

}    // namespace expressions::support

#endif
