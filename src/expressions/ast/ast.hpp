//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_AST_AST_HPP__
#define __EXPRESSIONS_AST_AST_HPP__

#include <expressions/support/boost/spirit.hpp>
#include <expressions/support/boost/variant.hpp>

#include <boost/fusion/adapted.hpp>
#include <boost/optional.hpp>

#include <string>
#include <vector>


namespace expressions::ast {

namespace x3 = boost::spirit::x3;

using ::expressions::support::boost::holds_alternative;
using ::expressions::support::boost::holds_any_of;

using ::expressions::support::boost::get;
using ::expressions::support::boost::get_if;

struct MonoState {};
struct Ellipsis {};
struct Null {};
struct Entry;
struct ImportPackage;
struct PackageName;

struct AssignStatement;
struct LazyAssignStatement;
struct AugAssignStatement;
struct ReturnStatement;
struct StatementList;
struct ExternFunctionDecl;
struct FunctionDef;
struct IfStatement;
struct ForStatement;
struct RangeBasedForStatement;
struct WhileStatement;
struct Pass;
struct Break;
struct Continue;

struct Expression;
struct Lambda;
struct BoolOp;
struct UnaryOp;
struct CompareOp;
struct CompareOpOperand;
struct BinOp;
struct BinOpIntermediate;

struct Call;
struct Argument;
struct KeywordArgument;
struct Subscript;

struct Name;
struct QuotedString;
struct String;
struct Date;
struct DateRange;
struct Tuple;
struct List;
struct Dict;
struct Set;

enum class BoolOpType : int32_t {
    kDefault,

    kAnd,
    kOr,

    kNot,
    kPlus,
    kMinus,
};

enum class CompareOpType : int32_t {
    kNone,

    kEQ,
    kNEQ,

    kLT,
    kLTE,

    kGT,
    kGTE,

    kIn,
    kNotIn,
};

enum class AliasType : int32_t {
    kNone,
    kAs,
};

enum class BinOpType : int32_t {
    kNone,

    kAdd,
    kSub,

    kMult,
    kTrueDiv,
    kFloorDiv,

    kMod,
    kPow,
};

using AtomType
    = x3::variant<MonoState, Ellipsis, Null, bool, int64_t, uint64_t, double,
                  x3::forward_ast<Name>, x3::forward_ast<String>,
                  x3::forward_ast<QuotedString>, x3::forward_ast<Date>,
                  x3::forward_ast<DateRange>>;

using CollectionType
    = x3::variant<MonoState, x3::forward_ast<Tuple>, x3::forward_ast<List>,
                  x3::forward_ast<Dict>, x3::forward_ast<Set>>;

using ArithmeticType
    = x3::variant<MonoState, x3::forward_ast<BoolOp>, x3::forward_ast<UnaryOp>,
                  x3::forward_ast<CompareOp>, x3::forward_ast<CompareOpOperand>,
                  x3::forward_ast<BinOp>>;

using ExpressionType
    = x3::variant<MonoState, x3::forward_ast<Call>, x3::forward_ast<Argument>,
                  x3::forward_ast<KeywordArgument>, x3::forward_ast<Lambda>,
                  x3::forward_ast<Subscript>, x3::forward_ast<Expression>>;

using StatementType = x3::variant<
    MonoState, x3::forward_ast<AssignStatement>,
    x3::forward_ast<LazyAssignStatement>, x3::forward_ast<AugAssignStatement>,
    x3::forward_ast<ReturnStatement>, x3::forward_ast<ExternFunctionDecl>,
    x3::forward_ast<FunctionDef>, x3::forward_ast<IfStatement>,
    x3::forward_ast<ForStatement>, x3::forward_ast<RangeBasedForStatement>,
    x3::forward_ast<WhileStatement>, x3::forward_ast<Pass>,
    x3::forward_ast<Break>, x3::forward_ast<Continue>,
    x3::forward_ast<StatementList>>;

using ValueType
    = x3::variant<MonoState, AtomType, CollectionType, ArithmeticType,
                  ExpressionType, StatementType, x3::forward_ast<Entry>>;

using Value = x3::variant<
    MonoState,

    // Atom
    Ellipsis, Null, bool, int64_t, uint64_t, double, x3::forward_ast<Name>,
    x3::forward_ast<String>, x3::forward_ast<QuotedString>,
    x3::forward_ast<Date>, x3::forward_ast<DateRange>,

    // Collection
    x3::forward_ast<Tuple>, x3::forward_ast<List>, x3::forward_ast<Dict>,
    x3::forward_ast<Set>,

    // Arithmetic
    x3::forward_ast<BoolOp>, x3::forward_ast<UnaryOp>,
    x3::forward_ast<CompareOp>, x3::forward_ast<CompareOpOperand>,
    x3::forward_ast<BinOp>, x3::forward_ast<BinOpIntermediate>,

    // Expression
    x3::forward_ast<Call>, x3::forward_ast<Argument>,
    x3::forward_ast<KeywordArgument>, x3::forward_ast<Lambda>,
    x3::forward_ast<Subscript>, x3::forward_ast<Expression>,

    // Function
    x3::forward_ast<ExternFunctionDecl>, x3::forward_ast<FunctionDef>,

    // Statement
    x3::forward_ast<AssignStatement>, x3::forward_ast<LazyAssignStatement>,
    x3::forward_ast<AugAssignStatement>, x3::forward_ast<ReturnStatement>,
    x3::forward_ast<IfStatement>, x3::forward_ast<ForStatement>,
    x3::forward_ast<RangeBasedForStatement>, x3::forward_ast<WhileStatement>,
    x3::forward_ast<StatementList>,

    // Nop
    x3::forward_ast<Pass>,

    // Control-flow
    x3::forward_ast<Break>, x3::forward_ast<Continue>,

    // Entry
    x3::forward_ast<ImportPackage>, x3::forward_ast<PackageName>,
    x3::forward_ast<Entry>>;

struct Name {
    std::string value {};
};

struct String {
    std::string value {};
};

struct QuotedString {
    std::string value {};
};

struct Date {
    int16_t year = 0;
    int8_t month = 0;
    int8_t day = 0;
};

struct DateRange {
    Date begin {};
    Date end {};
};

struct Tuple {
    std::vector<Value> values {};
};

struct List {
    std::vector<Value> values {};
};

struct DictItem {
    Value key;
    Value value;
};

struct Dict {
    std::vector<DictItem> items {};
};

struct Set {
    std::vector<Value> values {};
};

struct Call {
    Name name {};
    std::vector<Value> args {};
    // std::vector<KeywordArgument> kwargs {};
};

struct Argument {
    Value arg;
};

struct KeywordArgument {
    Name name;
    Value arg;
};

struct Subscript {
    Value name {};
    Value expr {};
};

struct BoolOp {
    BoolOpType op {BoolOpType::kDefault};
    std::vector<Value> operands;
};

struct UnaryOp {
    BoolOpType op {BoolOpType::kDefault};
    Value operand {};
};

struct CompareOpOperand {
    CompareOpType op {CompareOpType::kNone};
    Value operand {};
};

struct CompareOp {
    Value first {};
    std::vector<CompareOpOperand> rest {};
};

struct BinOp {
    Value left {};
    BinOpType op {BinOpType::kNone};
    Value right {};
};

struct BinOpOperand {
    BinOpType op {BinOpType::kNone};
    Value operand {};
};

struct BinOpIntermediate {
    Value first {};
    std::vector<BinOpOperand> rest {};
};

struct Expression {
    Value expr {};
};

struct Lambda {
    std::vector<Value> params {};
    Value expr {};
};

struct AssignStatement {
    Value target {};
    Value expr {};
};

struct LazyAssignStatement {
    Value target {};
    Value expr {};
};

struct AugAssignStatement {
    Value target {};
    BinOpType op {BinOpType::kNone};
    Value expr {};
};

struct ReturnStatement {
    boost::optional<Value> expr {};
};

struct StatementList {
    std::vector<Value> stmts;
};

struct ExternFunctionDecl {
    std::vector<Name> decorators {};
    Name name {};
    std::vector<Value> params {};
    Name return_type {};
};

struct FunctionDef {
    std::vector<Name> decorators {};
    Name name {};
    std::vector<Value> params {};
    Value body {};
};

struct IfStatement {
    Value condition {};
    Value body {};
    Value or_else {};
};

struct ForStatement {
    Value init {};
    Value condition {};
    Value iter {};
    Value body {};
};

struct RangeBasedForStatement {
    Value target {};
    Value iter {};
    Value body {};
};

struct WhileStatement {
    Value condition {};
    Value body {};
};

struct Pass {};

struct Break {};

struct Continue {};

struct ImportPackage {
    Name path;
};

struct PackageName {
    Name path;
};

struct Entry {
    // ImportPackages imports {};
    PackageName package {};
    Value node {};
};

}    // namespace expressions::ast

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Name, value)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::String, value)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Date, year, month, day)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::DateRange, begin, end)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::QuotedString, value)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Tuple, values)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::List, values)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::DictItem, key, value)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Dict, items)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Set, values)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Call, name, args)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Argument, arg)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::KeywordArgument, name, arg)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Subscript, name, expr)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::BoolOp, op, operands)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::UnaryOp, op, operand)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::CompareOpOperand, op, operand)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::CompareOp, first, rest)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::BinOp, left, op, right)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::BinOpOperand, op, operand)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::BinOpIntermediate, first, rest)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Lambda, params, expr)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Expression, expr)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::AssignStatement, target, expr)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::LazyAssignStatement, target, expr)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::AugAssignStatement, target, op,
                          expr)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::ReturnStatement, expr)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::StatementList, stmts)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::ExternFunctionDecl, decorators,
                          name, params, return_type)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::FunctionDef, decorators, name,
                          params, body)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::IfStatement, condition, body,
                          or_else)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::ForStatement, init, condition, iter,
                          body)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::RangeBasedForStatement, target,
                          iter, body)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::WhileStatement, condition, body)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::ImportPackage, path)
BOOST_FUSION_ADAPT_STRUCT(expressions::ast::PackageName, path)

BOOST_FUSION_ADAPT_STRUCT(expressions::ast::Entry, package, node)

#endif
