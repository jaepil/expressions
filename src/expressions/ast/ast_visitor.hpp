//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_AST_AST_VISITOR_HPP__
#define __EXPRESSIONS_AST_AST_VISITOR_HPP__

#include <expressions/ast/ast.hpp>

#include <expressions/support/boost/variant.hpp>

#include <boost/mp11.hpp>

#include <type_traits>


namespace expressions::ast {

template<typename T, typename R>
class ASTNodeVisitor : public boost::static_visitor<R> {
public:
    using ReturnType = R;

public:
    ASTNodeVisitor() = default;

    template<typename ValueType,
             std::enable_if_t<
                 boost::mp11::mp_contains<
                     Value::types, std::remove_cvref_t<ValueType>>::value,
                 int> = 0>
    decltype(auto) visit(ValueType&& node) const {
        return visit(Value {std::forward<ValueType>(node)});
    }
    decltype(auto) visit(const Value& node) const {
        return node.apply_visitor(get());
    }

    const T& get() const {
        return static_cast<const T&>(*this);
    }

    ReturnType operator()(const Null& node) const {
        return get().return_(node);
    }
    ReturnType operator()(bool value) const {
        return get().return_(value);
    }
    ReturnType operator()(int64_t value) const {
        return get().return_(value);
    }
    ReturnType operator()(uint64_t value) const {
        return get().return_(value);
    }
    ReturnType operator()(double value) const {
        return get().return_(value);
    }
    ReturnType operator()(const std::string& value) const {
        return get().return_(value);
    }
    ReturnType operator()(const Name& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const String& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const QuotedString& node) const {
        return get().return_(node);
    }

    ReturnType operator()(const Date& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const DateRange& node) const {
        return get().return_(node);
    }

    ReturnType operator()(const Tuple& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const List& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const Dict& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const Set& node) const {
        return get().return_(node);
    }

    ReturnType operator()(const Call& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const Argument& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const KeywordArgument& node) const {
        return get().return_(node);
    }

    ReturnType operator()(const CompareOp& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const BinOp& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const BinOpIntermediate& node) const {
        return get().return_(node);
    }

    ReturnType operator()(const UnaryOp& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const BoolOp& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const Lambda& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const Expression& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const AssignStatement& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const LazyAssignStatement& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const AugAssignStatement& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const ReturnStatement& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const StatementList& node) const {
        return get().return_(node);
    }

    ReturnType operator()(const FunctionDef& node) const {
        return get().return_(node);
    }

    ReturnType operator()(const IfStatement& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const ForStatement& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const RangeBasedForStatement& node) const {
        return get().return_(node);
    }
    ReturnType operator()(const WhileStatement& node) const {
        return get().return_(node);
    }

    ReturnType operator()(const Pass& node) const {
        (void)node;

        return get().return_(node);
    }
    ReturnType operator()(const Break& node) const {
        (void)node;

        return get().return_(node);
    }
    ReturnType operator()(const Continue& node) const {
        (void)node;

        return get().return_(node);
    }

    ReturnType operator()(const PackageName& node) const {
        return get().return_(node);
    }

    ReturnType operator()(const Entry& node) const {
        return get().return_(node);
    }
};

template<typename T, typename R>
class RecursiveASTNodeVisitor : public boost::static_visitor<R> {
public:
    using ReturnType = R;

public:
    RecursiveASTNodeVisitor() = default;

    template<typename U, typename ValueType,
             std::enable_if_t<
                 boost::mp11::mp_contains<
                     Value::types, std::remove_cvref_t<ValueType>>::value,
                 int> = 0>
    U visit(ValueType&& node) const {
        return ast::get<U>(visit(std::forward<ValueType>(node)));
    }
    template<typename ValueType,
             std::enable_if_t<
                 boost::mp11::mp_contains<
                     Value::types, std::remove_cvref_t<ValueType>>::value,
                 int> = 0>
    ReturnType visit(ValueType&& node) const {
        return static_cast<const T&>(*this)(node);
    }
    ReturnType visit(const Value& node) const {
        return node.apply_visitor(static_cast<const T&>(*this));
    }

    ReturnType operator()(const MonoState& node) const {
        return ReturnType {node};
    }
    ReturnType operator()(const Null& node) const {
        return ReturnType {node};
    }
    ReturnType operator()(bool value) const {
        return ReturnType {value};
    }
    ReturnType operator()(int64_t value) const {
        return ReturnType {value};
    }
    ReturnType operator()(uint64_t value) const {
        return ReturnType {value};
    }
    ReturnType operator()(double value) const {
        return ReturnType {value};
    }
    ReturnType operator()(const std::string& value) const {
        return ReturnType {value};
    }
    ReturnType operator()(const Name& node) const {
        return ReturnType {node};
    }
    ReturnType operator()(const String& node) const {
        return ReturnType {node};
    }
    ReturnType operator()(const QuotedString& node) const {
        return ReturnType {node};
    }

    ReturnType operator()(const Date& node) const {
        return ReturnType {node};
    }
    ReturnType operator()(const DateRange& node) const {
        return ReturnType {node};
    }

    ReturnType operator()(const Tuple& node) const {
        auto values = std::vector<Value> {};
        for (const auto& value : node.values) {
            values.emplace_back(visit(value));
        }

        return ReturnType {Tuple {std::move(values)}};
    }
    ReturnType operator()(const List& node) const {
        auto values = std::vector<Value> {};
        for (const auto& value : node.values) {
            values.emplace_back(visit(value));
        }

        return ReturnType {List {std::move(values)}};
    }
    ReturnType operator()(const Dict& node) const {
        auto values = std::vector<DictItem> {};
        for (const auto& item : node.items) {
            values.emplace_back(DictItem {visit(item.key), visit(item.value)});
        }

        return ReturnType {Dict {std::move(values)}};
    }
    ReturnType operator()(const Set& node) const {
        auto values = std::vector<Value> {};
        for (const auto& value : node.values) {
            values.emplace_back(visit(value));
        }

        return ReturnType {Set {std::move(values)}};
    }

    ReturnType operator()(const CompareOp& node) const {
        auto first = visit(node.first);
        auto rest = std::vector<CompareOpOperand> {};
        rest.reserve(node.rest.size());
        for (const auto& operand : node.rest) {
            rest.emplace_back(
                CompareOpOperand {operand.op, visit(operand.operand)});
        }

        return ReturnType {CompareOp {std::move(first), std::move(rest)}};
    }
    ReturnType operator()(const BinOp& node) const {
        return ReturnType {
            BinOp {visit(node.left), node.op, visit(node.right)}};
    }
    ReturnType operator()(const BinOpIntermediate& node) const {
        auto first = visit(node.first);
        auto rest = std::vector<BinOpOperand> {};
        rest.reserve(node.rest.size());
        for (const auto& operand : node.rest) {
            rest.emplace_back(
                BinOpOperand {operand.op, visit(operand.operand)});
        }

        return ReturnType {
            BinOpIntermediate {std::move(first), std::move(rest)}};
    }

    ReturnType operator()(const Call& node) const {
        std::vector<Value> args;
        args.reserve(node.args.size());
        for (const auto& arg : node.args) {
            args.emplace_back(visit(arg));
        }

        return ReturnType {Call {visit<Name>(node.name), std::move(args)}};
    }
    ReturnType operator()(const Argument& node) const {
        return ReturnType {Argument {visit(node.arg)}};
    }
    ReturnType operator()(const KeywordArgument& node) const {
        return ReturnType {KeywordArgument {node.name, visit(node.arg)}};
    }

    ReturnType operator()(const UnaryOp& node) const {
        auto value = visit(node.operand);

        return ReturnType {UnaryOp {node.op, std::move(value)}};
    }
    ReturnType operator()(const BoolOp& node) const {
        std::vector<Value> operands;
        operands.reserve(node.operands.size());
        for (const auto& operand : node.operands) {
            operands.emplace_back(visit(operand));
        }

        if (operands.size() == 1) {
            return ReturnType {std::move(operands[0])};
        }

        return ReturnType {BoolOp {node.op, std::move(operands)}};
    }
    ReturnType operator()(const Lambda& node) const {
        std::vector<Value> params;
        params.reserve(node.params.size());
        for (const auto& param : node.params) {
            params.emplace_back(visit(param));
        }

        return ReturnType {Lambda {std::move(params), visit(node.expr)}};
    }
    ReturnType operator()(const Expression& node) const {
        auto expr = visit(node.expr);

        return ReturnType {Expression {std::move(expr)}};
    }
    ReturnType operator()(const AssignStatement& node) const {
        auto target = visit(node.target);
        auto expr = visit(node.expr);

        return ReturnType {
            AssignStatement {std::move(target), std::move(expr)}};
    }
    ReturnType operator()(const LazyAssignStatement& node) const {
        auto target = visit(node.target);
        auto expr = visit(node.expr);

        return ReturnType {
            LazyAssignStatement {std::move(target), std::move(expr)}};
    }
    ReturnType operator()(const AugAssignStatement& node) const {
        auto target = visit(node.target);
        auto expr = visit(node.expr);

        return ReturnType {
            AugAssignStatement {std::move(target), node.op, std::move(expr)}};
    }
    ReturnType operator()(const ReturnStatement& node) const {
        return ReturnType {ReturnStatement {visit(node.expr)}};
    }
    ReturnType operator()(const StatementList& node) const {
        auto new_stmts = std::vector<Value> {};
        new_stmts.reserve(node.stmts.size());
        for (const auto& n : node.stmts) {
            new_stmts.emplace_back(visit(n));
        }

        return ReturnType {StatementList {std::move(new_stmts)}};
    }

    ReturnType operator()(const FunctionDef& node) const {
        auto params = std::vector<Value> {};
        params.reserve(node.params.size());
        for (const auto& p : node.params) {
            params.emplace_back(visit(p));
        }

        return ReturnType {FunctionDef {visit<Name>(node.name),
                                        std::move(params), visit(node.body)}};
    }

    ReturnType operator()(const IfStatement& node) const {
        return ReturnType {IfStatement {visit(node.condition), visit(node.body),
                                        visit(node.or_else)}};
    }
    ReturnType operator()(const ForStatement& node) const {
        return ReturnType {ForStatement {
            visit(node.init), visit(node.condition), visit(node.iter),
            visit(node.body), visit(node.or_else)}};
    }
    ReturnType operator()(const RangeBasedForStatement& node) const {
        return ReturnType {
            RangeBasedForStatement {visit(node.target), visit(node.iter),
                                    visit(node.body), visit(node.or_else)}};
    }
    ReturnType operator()(const WhileStatement& node) const {
        return ReturnType {WhileStatement {
            visit(node.condition), visit(node.body), visit(node.or_else)}};
    }

    ReturnType operator()(const Pass& node) const {
        (void)node;

        return ReturnType {Pass {}};
    }
    ReturnType operator()(const Break& node) const {
        (void)node;

        return ReturnType {Break {}};
    }
    ReturnType operator()(const Continue& node) const {
        (void)node;

        return ReturnType {Continue {}};
    }

    ReturnType operator()(const PackageName& node) const {
        return ReturnType {node};
    }

    ReturnType operator()(const Entry& node) const {
        return ReturnType {Entry {node.package, visit(node.node)}};
    }
};

}    // namespace expressions::ast

#endif
