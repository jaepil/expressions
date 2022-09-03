//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_INTERPRETER_AST_INTERPRETER_HPP__
#define __EXPRESSIONS_INTERPRETER_AST_INTERPRETER_HPP__

#include <expressions/ast/ast.hpp>

#include <expressions/exception/throw_exception.hpp>

#include <expressions/support/boost/variant.hpp>

#include <fmt/format.h>
#include <boost/mp11.hpp>
#include <boost/type_index.hpp>

#include <map>
#include <set>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <vector>


namespace expressions::interpreter {

class BreakException : public std::exception {
public:
    using exception::exception;
};

class ContinueException : public std::exception {
public:
    using exception::exception;
};

class ReturnException : public std::exception {
public:
    using exception::exception;
};

class ProgramTerminated : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

struct Null {
    bool operator==(const Null&) const = default;
    bool operator!=(const Null&) const = default;

    template<typename T>
    bool operator==(const T&) const {
        return false;
    }
    template<typename T>
    bool operator!=(const T&) const {
        return false;
    }
    template<typename T>
    bool operator<(const T&) const {
        return false;
    }
    template<typename T>
    bool operator<=(const T&) const {
        return false;
    }
    template<typename T>
    bool operator>(const T&) const {
        return false;
    }
    template<typename T>
    bool operator>=(const T&) const {
        return false;
    }
};

struct Name {
    std::string value;

    bool operator==(const Name& rhs) const {
        return value == rhs.value;
    }
    bool operator!=(const Name& rhs) const {
        return value != rhs.value;
    }
    bool operator<(const Name& rhs) const {
        return value < rhs.value;
    }
    bool operator<=(const Name& rhs) const {
        return value <= rhs.value;
    }
    bool operator>(const Name& rhs) const {
        return value > rhs.value;
    }
    bool operator>=(const Name& rhs) const {
        return value >= rhs.value;
    }
};

struct String {
    std::string value;

    bool operator==(const String& rhs) const {
        return value == rhs.value;
    }
    bool operator!=(const String& rhs) const {
        return value != rhs.value;
    }
    bool operator<(const String& rhs) const {
        return value < rhs.value;
    }
    bool operator<=(const String& rhs) const {
        return value <= rhs.value;
    }
    bool operator>(const String& rhs) const {
        return value > rhs.value;
    }
    bool operator>=(const String& rhs) const {
        return value >= rhs.value;
    }
};

struct Date {
    int16_t year = 0;
    int8_t month = 0;
    int8_t day = 0;

    bool operator==(const Date& rhs) const {
        return std::make_tuple(year, month, day)
               == std::make_tuple(rhs.year, rhs.month, rhs.day);
    }
    bool operator!=(const Date& rhs) const {
        return std::make_tuple(year, month, day)
               != std::make_tuple(rhs.year, rhs.month, rhs.day);
    }
    bool operator<(const Date& rhs) const {
        return std::make_tuple(year, month, day)
               < std::make_tuple(rhs.year, rhs.month, rhs.day);
    }
    bool operator<=(const Date& rhs) const {
        return std::make_tuple(year, month, day)
               <= std::make_tuple(rhs.year, rhs.month, rhs.day);
    }
    bool operator>(const Date& rhs) const {
        return std::make_tuple(year, month, day)
               > std::make_tuple(rhs.year, rhs.month, rhs.day);
    }
    bool operator>=(const Date& rhs) const {
        return std::make_tuple(year, month, day)
               >= std::make_tuple(rhs.year, rhs.month, rhs.day);
    }
};

struct DateRange {
    Date begin;
    Date end;

    bool operator==(const DateRange& rhs) const {
        return begin == rhs.begin && end == rhs.end;
    }
    bool operator!=(const DateRange& rhs) const {
        return begin != rhs.begin || end != rhs.end;
    }
    bool operator<(const DateRange& rhs) const {
        return begin < rhs.begin;
    }
    bool operator<=(const DateRange& rhs) const {
        return begin <= rhs.begin;
    }
    bool operator>(const DateRange& rhs) const {
        return end > rhs.end;
    }
    bool operator>=(const DateRange& rhs) const {
        return end >= rhs.end;
    }
};

struct Code {
    ast::Value code;

    bool operator==(const Code&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '==' for type 'Code'"));
    }
    bool operator!=(const Code&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '!=' for type 'Code'"));
    }
    bool operator<(const Code&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '<' for type 'Code'"));
    }
    bool operator<=(const Code&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '<=' for type 'Code'"));
    }
    bool operator>(const Code&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '>' for type 'Code'"));
    }
    bool operator>=(const Code&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '>=' for type 'Code'"));
    }
};

struct Lambda {
    std::vector<ast::Value> params {};
    ast::Value body {};

    bool operator==(const Lambda&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '==' for type 'Lambda'"));
    }
    bool operator!=(const Lambda&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '!=' for type 'Lambda'"));
    }
    bool operator<(const Lambda&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '<' for type 'Lambda'"));
    }
    bool operator<=(const Lambda&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '<=' for type 'Lambda'"));
    }
    bool operator>(const Lambda&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '>' for type 'Lambda'"));
    }
    bool operator>=(const Lambda&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '>=' for type 'Lambda'"));
    }
};

struct Function {
    Name name {};
    std::vector<ast::Value> params {};
    ast::Value body {};

    bool operator==(const Function&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '==' for type 'Function'"));
    }
    bool operator!=(const Function&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '!=' for type 'Function'"));
    }
    bool operator<(const Function&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '<' for type 'Function'"));
    }
    bool operator<=(const Function&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '<=' for type 'Function'"));
    }
    bool operator>(const Function&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '>' for type 'Function'"));
    }
    bool operator>=(const Function&) const {
        THROW_EXCEPTION(
            std::logic_error("Unsupported operator '>=' for type 'Function'"));
    }
};

template<typename T>
struct Tuple : std::vector<T> {};

template<typename T>
struct Vector : std::vector<T> {};

template<typename T>
struct Set : std::set<T> {};

template<typename K, typename V>
struct Map : std::map<K, V> {};

using BoxedValue = boost::make_recursive_variant<
    Null, bool, int64_t, uint64_t, double, Name, String, Date, DateRange, Code,
    Lambda, Function, Tuple<boost::recursive_variant_>,
    Vector<boost::recursive_variant_>, Set<boost::recursive_variant_>,
    Map<boost::recursive_variant_, boost::recursive_variant_>>::type;

class ASTInterpreter : public boost::static_visitor<BoxedValue> {
public:
    ASTInterpreter() = default;

    using ReturnType = BoxedValue;

    ReturnType execute(const ast::Entry& node) const {
        return visit_(node);
    }

private:
    template<typename T, typename ValueType,
             std::enable_if_t<
                 boost::mp11::mp_contains<
                     ast::Value::types, std::remove_cvref_t<ValueType>>::value,
                 int> = 0>
    T visit_(ValueType&& node) const {
        return ast::get<T>(visit_(std::forward<ValueType>(node)));
    }
    template<typename ValueType>
    ReturnType visit_(ValueType&& node) const {
        if constexpr (std::same_as<std::remove_cvref_t<ValueType>,
                                   ast::Value>) {
            return node.apply_visitor(*this);

        } else {
            return (*this)(std::forward<ValueType>(node));
        }
    }
    ReturnType visit_(const ast::Value& node) const {
        return node.apply_visitor(*this);
    }

public:
    ReturnType operator()(const ast::MonoState& node) const;

    ReturnType operator()(const ast::Null& node) const;
    ReturnType operator()(bool value) const;

    ReturnType operator()(int64_t value) const;
    ReturnType operator()(uint64_t value) const;
    ReturnType operator()(double value) const;

    ReturnType operator()(const ast::Name& node) const;
    ReturnType operator()(const ast::String& node) const;
    ReturnType operator()(const ast::QuotedString& node) const;

    ReturnType operator()(const ast::Date& node) const;
    ReturnType operator()(const ast::DateRange& node) const;

    ReturnType operator()(const ast::Tuple& node) const;
    ReturnType operator()(const ast::List& node) const;
    ReturnType operator()(const ast::Dict& node) const;
    ReturnType operator()(const ast::Set& node) const;

    ReturnType operator()(const ast::CompareOp& node) const;
    ReturnType operator()(const ast::BinOp& node) const;
    ReturnType operator()(const ast::BinOpIntermediate& node) const {
        auto name = boost::typeindex::type_id<decltype(node)>().pretty_name();
        THROW_EXCEPTION(std::runtime_error(fmt::format(
            "[Interpreter] Cannot execute the AST node type '{}'", name)));
    }

    ReturnType operator()(const ast::Call& node) const;
    ReturnType operator()(const ast::Argument& node) const;
    ReturnType operator()(const ast::KeywordArgument& node) const;

    ReturnType operator()(const ast::UnaryOp& node) const;
    ReturnType operator()(const ast::BoolOp& node) const;

    ReturnType operator()(const ast::Lambda& node) const;
    ReturnType operator()(const ast::Expression& node) const;
    ReturnType operator()(const ast::AssignStatement& node) const;
    ReturnType operator()(const ast::LazyAssignStatement& node) const;
    ReturnType operator()(const ast::AugAssignStatement& node) const;
    ReturnType operator()(const ast::ReturnStatement& node) const;
    ReturnType operator()(const ast::StatementList& node) const;

    ReturnType operator()(const ast::FunctionDef& node) const;

    ReturnType operator()(const ast::IfStatement& node) const;
    ReturnType operator()(const ast::ForStatement& node) const;
    ReturnType operator()(const ast::RangeBasedForStatement& node) const;
    ReturnType operator()(const ast::WhileStatement& node) const;

    ReturnType operator()(const ast::Pass& node) const;
    ReturnType operator()(const ast::Break& node) const;
    ReturnType operator()(const ast::Continue& node) const;

    ReturnType operator()(const ast::PackageName& node) const;

    ReturnType operator()(const ast::Entry& node) const;

private:
    ReturnType execute_bin_op_(ast::BinOpType op, BoxedValue&& left,
                               BoxedValue&& right) const;
    bool check_branch_condition_(const BoxedValue& value) const;
    BoxedValue return_value_and_reset_() const;

private:
    using Context = std::unordered_map<std::string, BoxedValue>;
    mutable Context context_ {};
    mutable std::vector<Context> stack_ {};
    mutable BoxedValue return_value_ {};
};

}    // namespace expressions::interpreter

#endif
