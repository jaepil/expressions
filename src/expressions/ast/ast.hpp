//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_AST_AST_HPP__
#define __EXPRESSIONS_AST_AST_HPP__

#include <expressions/common/box.hpp>

#include <boost/fusion/adapted.hpp>

#include <optional>
#include <string>
#include <variant>
#include <vector>


namespace expressions::ast {

enum class BoolOpType : int32_t {
    kDefault,

    kAnd,
    kOr,
    kShould,

    kNot,
    kPlus,
    kMinus,
    kFilter,
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

using Statement
    = std::variant<Box<struct CompoundStatement>, Box<struct SimpleStatement>>;

using Value = std::variant<
    // Atoms
    Box<struct None>, Box<struct Int64>, Box<struct UInt64>, Box<struct Double>,
    Box<struct String>, Box<struct QuotedString>,

    // Statements
    Box<struct Module>, Box<struct Statements>, Statement,
    Box<struct Expressions>, Box<struct Expression>,
    Box<struct AssignmentStatement>, Box<struct AugAssignmentStatement>,
    Box<struct ReturnStatement>, Box<struct PassStatement>,
    Box<struct BreakStatement>, Box<struct ContinueStatement>,

    // Expressions
    Box<struct BinOp>, Box<struct Factor>, Box<struct Power>>;

struct None {};

struct Int64 {
    int64_t n {};
    std::string s {};
};

struct UInt64 {
    uint64_t n {};
    std::string s {};
};

struct Double {
    double n {};
    std::string s {};
};

struct String {
    std::string value;
};

struct QuotedString {
    std::string value;
};

struct Module {
    Box<Statements> stmts;
};

struct Statements {
    std::vector<Statement> stmts;
};

struct CompoundStatement {};

struct SimpleStatement {};

struct Expressions {
    std::vector<Value> exprs;
};

struct Expression {
    Value expr;
};

struct AssignmentStatement {
    Value target;
    Value expr;
};

struct AugAssignmentStatement {
    Value target;
    BinOpType op {BinOpType::kNone};
    Value expr;
};

struct ReturnStatement {
    std::optional<Value> expr;
};

struct PassStatement {};

struct BreakStatement {};

struct ContinueStatement {};

struct BinOp {
    Expression left;
    BinOpType op {BinOpType::kNone};
    Expression right;
};

struct Factor {};

struct Power {};

}    // namespace expressions::ast

BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::None)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::Int64, n, s)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::UInt64, n, s)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::Double, n, s)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::String, value)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::QuotedString, value)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::Module, stmts)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::Statements, stmts)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::CompoundStatement)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::SimpleStatement)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::Expressions, exprs)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::Expression, expr)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::AssignmentStatement, target, expr)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::AugAssignmentStatement, target,
                          op, expr)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::ReturnStatement, expr)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::PassStatement)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::BreakStatement)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::ContinueStatement)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::BinOp, left, op, right)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::Factor)
BOOST_FUSION_ADAPT_STRUCT(::expressions::ast::Power)

#endif
