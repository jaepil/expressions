//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_EXCEPTION_THROW_EXCEPTION_HPP__
#define __EXPRESSIONS_EXCEPTION_THROW_EXCEPTION_HPP__

#ifdef ENABLE_BOOST_STACK_TRACE
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <boost/exception/info.hpp>
#include <boost/throw_exception.hpp>

#include <boost/stacktrace.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#endif

#include <cstdint>
#include <stdexcept>


namespace expressions::exception {

namespace detail {
#ifdef ENABLE_BOOST_STACK_TRACE
using StackTrace = ::boost::stacktrace::stacktrace;
using StackTraceInfo = ::boost::error_info<struct stacktrace_tag, StackTrace>;
#else
struct StackTrace {};
#endif

#if defined(ENABLE_BOOST_STACK_TRACE) && defined(BOOST_CURRENT_LOCATION)
template<typename E>
[[noreturn]] constexpr void throw_exception(
    const E& x, const ::boost::source_location& location,
    const StackTrace& trace) {
    ::boost::throw_exception(
        ::boost::enable_error_info(x)
        << ::boost::throw_function {location.function_name()}
        << ::boost::throw_file {location.file_name()}
        << ::boost::throw_line {static_cast<int32_t>(location.line())}
        << StackTraceInfo {trace});
}

template<typename E>
[[noreturn]] constexpr void throw_non_std_exception(
    const E& x, const ::boost::source_location& location,
    const StackTrace& trace) {
    auto exception
        = ::boost::enable_error_info(x)
          << ::boost::throw_function {location.function_name()}
          << ::boost::throw_file {location.file_name()}
          << ::boost::throw_line {static_cast<int32_t>(location.line())}
          << StackTraceInfo {trace};
    throw exception;
}
#else
template<typename E>
[[noreturn]] constexpr void throw_exception(const E& x,
                                            const char* current_function,
                                            const char* file, int32_t line,
                                            const StackTrace& trace) {
    (void)current_function;
    (void)file;
    (void)line;
    (void)trace;

    throw x;
}

template<typename E>
[[noreturn]] constexpr void throw_non_std_exception(
    const E& x, const char* current_function, const char* file, int32_t line,
    const StackTrace& trace) {
    (void)current_function;
    (void)file;
    (void)line;
    (void)trace;

    throw x;
}
#endif
}    // namespace detail

#if defined(ENABLE_BOOST_STACK_TRACE) && defined(BOOST_CURRENT_LOCATION)
#define THROW_EXCEPTION(x)                             \
    ::expressions::exception::detail::throw_exception( \
        x, BOOST_CURRENT_LOCATION,                     \
        ::expressions::exception::detail::StackTrace {})

#define THROW_NON_STD_EXCEPTION(x)                             \
    ::expressions::exception::detail::throw_non_std_exception( \
        x, BOOST_CURRENT_LOCATION,                             \
        ::expressions::exception::detail::StackTrace {})
#else
#define THROW_EXCEPTION(x)                             \
    ::expressions::exception::detail::throw_exception( \
        x, __PRETTY_FUNCTION__, __FILE__, __LINE__,    \
        ::expressions::exception::detail::StackTrace {})

#define THROW_NON_STD_EXCEPTION(x)                             \
    ::expressions::exception::detail::throw_non_std_exception( \
        x, __PRETTY_FUNCTION__, __FILE__, __LINE__,            \
        ::expressions::exception::detail::StackTrace {})
#endif

}    // namespace expressions::exception

#endif
