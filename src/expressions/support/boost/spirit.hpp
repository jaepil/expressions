//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_SUPPORT_BOOST_SPIRIT_HPP__
#define __EXPRESSIONS_SUPPORT_BOOST_SPIRIT_HPP__

// #define BOOST_SPIRIT_X3_DEBUG

// clang-format off
#ifdef BOOST_SPIRIT_X3_DEBUG
    #ifdef NDEBUG
        #pragma message("Warning: BOOST_SPIRIT_X3_DEBUG is enabled for release build.")
    #endif
#endif
// clang-format on

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-copy"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/char/unicode.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include <concepts>


namespace expressions::support::boost::spirit::x3 {

namespace {
namespace x3 = ::boost::spirit::x3;

template<typename T>
struct as_type {
    template<typename...>
    struct tag {};

    template<typename P>
    auto operator[](P p) const {
        return x3::rule<tag<T, P>, T> {"as"} = x3::as_parser(p);
    }
};

template<typename Tag>
struct set_context_type {
    template<typename P>
    auto operator[](P p) const {
        auto action = [](auto& ctx) {
            x3::get<Tag>(ctx) = x3::_attr(ctx);
        };
        return x3::omit[p[action]];
    }
};

template<typename Tag>
struct lazy_type : x3::parser<lazy_type<Tag>> {
    using attribute_type = typename Tag::attribute_type;

    template<typename It, typename Ctx, typename RCtx, typename Attr>
    bool parse(It& first, It last, Ctx& ctx, RCtx& rctx, Attr& attr) const {
        return x3::as_parser(Tag {}(ctx))
            .parse(first, last, std::forward<Ctx>(ctx),
                   std::forward<RCtx>(rctx), attr);
    }
};
}    // namespace

template<typename T>
constexpr auto as = as_type<T> {};    // NOLINT(readability-identifier-naming)

template<typename T>
constexpr auto set_context    // NOLINT(readability-identifier-naming)
    = set_context_type<T> {};

template<typename T>
constexpr auto lazy    // NOLINT(readability-identifier-naming)
    = lazy_type<T> {};

constexpr auto distinct    // NOLINT(readability-identifier-naming)
    = [](auto p) {
          // Make sure we have a whole word.
          return x3::lexeme[p >> !(x3::unicode::alnum | '_')];
      };

template<typename T, uint32_t Radix = 10, uint32_t MinDigits = 1,
         int32_t MaxDigits = -1>
struct ranged_int_parser
    : x3::parser<ranged_int_parser<T, Radix, MinDigits, MaxDigits>> {
    static_assert((Radix == 2 || Radix == 8 || Radix == 10 || Radix == 16),
                  "Error Unsupported Radix");

    using attribute_type = T;
    static constexpr bool has_attribute = true;    // NOLINT

    ranged_int_parser(T from, T to) : from_(from), to_(to) {
    }

    template<typename Iterator, typename Context, typename Attribute>
    bool parse(Iterator& first, const Iterator& last, const Context& context,
               x3::unused_type, Attribute& attr) const {
        using extract = x3::extract_int<T, Radix, MinDigits, MaxDigits>;
        x3::skip_over(first, last, context);
        return extract::call(first, last, attr);
    }
    template<typename Iterator, typename Context, typename Attribute,
             std::enable_if_t<std::is_convertible_v<Attribute, T>, int> = 0>
    bool parse(Iterator& first, const Iterator& last, const Context& context,
               x3::unused_type, Attribute& attr) const {
        using extract = x3::extract_int<T, Radix, MinDigits, MaxDigits>;
        x3::skip_over(first, last, context);
        auto hit = extract::call(first, last, attr);
        if (!hit) {
            return false;
        }

        return attr >= from_ && attr <= to_;
    }

private:
    T from_;
    T to_;
};

// NOLINTNEXTLINE(readability-identifier-naming)
static const auto strict_double
    = x3::real_parser<double, x3::strict_real_policies<double>> {};

}    // namespace expressions::support::boost::spirit::x3

namespace boost::spirit::x3 {

using ::expressions::support::boost::spirit::x3::as;
using ::expressions::support::boost::spirit::x3::distinct;
using ::expressions::support::boost::spirit::x3::lazy_type;
using ::expressions::support::boost::spirit::x3::set_context;

using ::expressions::support::boost::spirit::x3::ranged_int_parser;

using ::expressions::support::boost::spirit::x3::strict_double;

}    // namespace boost::spirit::x3

#endif
