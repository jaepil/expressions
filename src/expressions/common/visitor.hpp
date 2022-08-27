//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_COMMON_VISITOR_HPP__
#define __EXPRESSIONS_COMMON_VISITOR_HPP__

#include <expressions/exception/throw_exception.hpp>

#include <boost/mp11.hpp>
#include <boost/variant/multivisitors.hpp>
#include <boost/variant/variant.hpp>

#include <type_traits>
#include <utility>
#include <variant>


namespace expressions {

namespace detail {
template<typename T>
struct is_variant : std::false_type {};

template<typename... Args>
struct is_variant<std::variant<Args...>> : std::true_type {};

template<typename... Args>
struct is_variant<boost::variant<Args...>> : std::true_type {};

template<typename T>
// NOLINTNEXTLINE
inline constexpr auto is_variant_v = is_variant<std::remove_cvref_t<T>>::value;

template<typename T>
concept VariantTypes = is_variant_v<T>;

enum class VariantType : int32_t {
    kUnknown = 0,
    kStd = 1,
    kBoost = 2,
};

template<typename T>
struct variant_type {
    // NOLINTNEXTLINE
    inline static constexpr auto value = VariantType::kUnknown;
};

template<typename... Args>
struct variant_type<std::variant<Args...>> {
    // NOLINTNEXTLINE
    inline static constexpr auto value = VariantType::kStd;
};

template<typename... Args>
struct variant_type<boost::variant<Args...>> {
    // NOLINTNEXTLINE
    inline static constexpr auto value = VariantType::kBoost;
};

template<typename T>
// NOLINTNEXTLINE
inline constexpr auto variant_type_v
    = variant_type<std::remove_cvref_t<T>>::value;

template<
    typename Visitor, VariantTypes Visitable,
    std::enable_if_t<variant_type_v<Visitable> == VariantType::kStd, int> = 0>
inline constexpr auto visit(Visitor&& visitor, Visitable&& visitable) {
    return std::visit(std::forward<Visitor>(visitor),
                      std::forward<Visitable>(visitable));
}

template<
    typename Visitor, VariantTypes Visitable,
    std::enable_if_t<variant_type_v<Visitable> == VariantType::kBoost, int> = 0>
inline constexpr auto visit(Visitor&& visitor, Visitable&& visitable) {
    return boost::apply_visitor(std::forward<Visitor>(visitor),
                                std::forward<Visitable>(visitable));
}

template<typename Visitor, VariantTypes Visitable,
         std::enable_if_t<variant_type_v<Visitable> == VariantType::kUnknown,
                          int> = 0>
inline constexpr auto visit([[maybe_unused]] Visitor&& visitor,
                            [[maybe_unused]] Visitable&& visitable) {
    THROW_EXCEPTION(std::logic_error("Unknown variant type."));
}

template<typename Visitor, VariantTypes... Visitable,
         std::enable_if_t<variant_type_v<Visitable...> == VariantType::kStd,
                          int> = 0>
inline constexpr auto visit(Visitor&& visitor, Visitable&&... visitables) {
    return std::visit(std::forward<Visitor>(visitor),
                      std::forward<Visitable>(visitables)...);
}

template<typename Visitor, VariantTypes... Visitable,
         std::enable_if_t<variant_type_v<Visitable...> == VariantType::kBoost,
                          int> = 0>
inline constexpr auto visit(Visitor&& visitor, Visitable&&... visitables) {
    return boost::apply_visitor(std::forward<Visitor>(visitor),
                                std::forward<Visitable>(visitables)...);
}

template<typename Visitor, VariantTypes... Visitable,
         std::enable_if_t<variant_type_v<Visitable...> == VariantType::kUnknown,
                          int> = 0>
inline constexpr auto visit([[maybe_unused]] Visitor&& visitor,
                            [[maybe_unused]] Visitable&&... visitables) {
    THROW_EXCEPTION(std::logic_error("Unknown variant type."));
}
}    // namespace detail

template<typename... Ts>
struct Visitor : Ts... {
    using Ts::operator()...;

    template<typename F, detail::VariantTypes Visitable>
    constexpr auto visit(F&& visitor, Visitable&& visitable) const {
        return detail::visit(std::forward<F>(visitor),
                             std::forward<Visitable>(visitable));
    }

    template<typename... Args>
    constexpr auto visit(Args&&... args) const {
        return (*this)(std::forward<Args>(args)...);
    }
};

template<typename... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

template<typename... Ts>
struct SelfVisitableVisitor : Ts... {
    using Ts::operator()...;

    template<typename F, detail::VariantTypes Visitable>
    constexpr auto visit(F&& visitor, Visitable&& visitable) const {
        using Self = std::remove_cvref_t<decltype(visitor)>;
        using R = decltype(Self::operator()(
            std::forward<F>(visitor), std::forward<Visitable>(visitable)));

#ifdef __GNUC__
        // Workaround for GCC 10's bug.
        return detail::visit(
            [&](auto&& v) -> R {
                return Self::operator()(*this, v);
            },
            std::forward<Visitable>(visitable));
#else
        return detail::visit(
            [&visitor](auto&& v) -> R {
                return Self::operator()(visitor, v);
            },
            std::forward<Visitable>(visitable));
#endif
    }

    template<detail::VariantTypes Visitable>
    constexpr auto visit(Visitable&& visitable) const {
        return visit(*this, std::forward<Visitable>(visitable));
    }

    template<typename... Args>
    constexpr auto visit(Args&&... args) const {
        using Self = std::remove_cvref_t<decltype(*this)>;

        return Self::operator()(*this, std::forward<Args>(args)...);
    }
};

template<typename... Ts>
SelfVisitableVisitor(Ts...) -> SelfVisitableVisitor<Ts...>;

template<typename... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template<typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template<typename F, typename... V>
constexpr auto visit(F&& f, V&&... v) {
    using R = boost::mp11::mp_unique<boost::mp11::mp_product_q<
        boost::mp11::mp_bind_front<std::invoke_result_t, F>,
        std::remove_reference_t<V>...>>;

    return std::visit(
        [&](auto&&... x) {
            return R(std::forward<F>(f)(std::forward<decltype(x)>(x)...));
        },
        std::forward<V>(v)...);
}

}    // namespace expressions

#endif
