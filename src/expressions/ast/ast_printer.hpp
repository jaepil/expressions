//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_AST_AST_PRINTER_HPP__
#define __EXPRESSIONS_AST_AST_PRINTER_HPP__

#include <expressions/ast/ast_visitor.hpp>

#include <fmt/format.h>
#include <magic_enum.hpp>


namespace expressions::ast {

class ASTPrinter : public ASTNodeVisitor<ASTPrinter, std::string> {
public:
    ASTPrinter() = default;

    ReturnType operator()(const ast::MonoState& node) const {
        (void)node;

        return fmt::format("MonoState[]");
    }

    ReturnType operator()(const Ellipsis& node) const {
        (void)node;

        return fmt::format("Ellipsis[]");
    }
    ReturnType operator()(const Null& node) const {
        (void)node;

        return fmt::format("Null[]");
    }
    ReturnType operator()(bool value) const {
        return fmt::format("Bool[value={}]", value);
    }
    ReturnType operator()(int64_t value) const {
        return fmt::format("Int64[value={}]", value);
    }
    ReturnType operator()(uint64_t value) const {
        return fmt::format("UInt64[value={}]", value);
    }
    ReturnType operator()(double value) const {
        return fmt::format("Double[value={}]", value);
    }
    ReturnType operator()(const std::string& value) const {
        return fmt::format("str[value=\"{}\"]", value);
    }
    ReturnType operator()(const Name& node) const {
        return fmt::format("Name[value={}]", node.value);
    }
    ReturnType operator()(const String& node) const {
        return fmt::format("String[value={}]", node.value);
    }
    ReturnType operator()(const QuotedString& node) const {
        return fmt::format("QuotedString[value=\"{}\"]", node.value);
    }

    ReturnType operator()(const Date& node) const {
        return fmt::format("Date[year={}, month={}, day={}]", node.year,
                           node.month, node.day);
    }
    ReturnType operator()(const DateRange& node) const {
        return fmt::format("DateRange[begin={}, end={}]", visit(node.begin),
                           visit(node.end));
    }

    ReturnType operator()(const Tuple& node) const {
        auto values = std::vector<ReturnType> {};
        values.reserve(node.values.size());
        std::transform(node.values.begin(), node.values.end(),
                       std::back_inserter(values), [&](const auto& v) {
                           return visit(v);
                       });

        return fmt::format("Tuple[value=[{}]]", fmt::join(values, ", "));
    }
    ReturnType operator()(const List& node) const {
        auto values = std::vector<ReturnType> {};
        values.reserve(node.values.size());
        std::transform(node.values.begin(), node.values.end(),
                       std::back_inserter(values), [&](const auto& v) {
                           return visit(v);
                       });

        return fmt::format("List[value=[{}]]", fmt::join(values, ", "));
    }
    ReturnType operator()(const Dict& node) const {
        auto keys = std::vector<ReturnType> {};
        keys.reserve(node.items.size());
        auto values = std::vector<ReturnType> {};
        values.reserve(node.items.size());
        std::for_each(node.items.begin(), node.items.end(), [&](const auto& v) {
            auto key = visit(v.key);
            auto value = visit(v.value);

            keys.emplace_back(std::move(key));
            values.emplace_back(std::move(value));
        });

        return fmt::format("Dict[keys=[{}], values=[{}]]",
                           fmt::join(keys, ", "), fmt::join(values, ", "));
    }
    ReturnType operator()(const Set& node) const {
        auto values = std::vector<ReturnType> {};
        values.reserve(node.values.size());
        std::transform(node.values.begin(), node.values.end(),
                       std::back_inserter(values), [&](const auto& v) {
                           return visit(v);
                       });

        return fmt::format("Set[value=[{}]]", fmt::join(values, ", "));
    }

    ReturnType operator()(const Call& node) const {
        auto args = std::vector<ReturnType> {};
        args.reserve(node.args.size());
        std::transform(node.args.begin(), node.args.end(),
                       std::back_inserter(args), [&](const auto& v) {
                           return visit(v);
                       });

        return fmt::format("Call[callable={}, args=[{}]]", visit(node.name),
                           fmt::join(args, ", "));
    }
    ReturnType operator()(const Argument& node) const {
        return fmt::format("Argument[arg={}]", visit(node.arg));
    }
    ReturnType operator()(const KeywordArgument& node) const {
        return fmt::format("KeywordArgument[name={}, arg={}]", visit(node.name),
                           visit(node.arg));
    }

    ReturnType operator()(const Subscript& node) const {
        return fmt::format("Subscript[name={}, expr={}", visit(node.name),
                           visit(node.expr));
    }

    ReturnType operator()(const CompareOp& node) const {
        auto first = visit(node.first);
        auto rest = std::vector<ReturnType> {};
        rest.reserve(node.rest.size());
        for (const auto& operand : node.rest) {
            auto rhs = visit(operand);
            rest.emplace_back(std::move(rhs));
        }

        return fmt::format("CompareOp[first={}, rest=[{}]]", first,
                           fmt::join(rest, ", "));
    }
    ReturnType operator()(const CompareOpOperand& node) const {
        auto op = magic_enum::enum_name(node.op);
        auto operand = visit(node.operand);

        return fmt::format("CompareOpOperand[op={}, operand={}]", op, operand);
    }

    ReturnType operator()(const BinOp& node) const {
        auto left = visit(node.left);
        auto op = magic_enum::enum_name(node.op);
        auto right = visit(node.right);

        return fmt::format("BinOp[left={}, op={}, right={}]", left, op, right);
    }
    ReturnType operator()(const BinOpIntermediate& node) const {
        (void)node;

        return fmt::format("BinOpIntermediate[]");
    }

    ReturnType operator()(const UnaryOp& node) const {
        auto op = magic_enum::enum_name(node.op);
        auto operand = visit(node.operand);

        return fmt::format("UnaryOp[op={}, operand={}]", op, operand);
    }
    ReturnType operator()(const BoolOp& node) const {
        auto op = magic_enum::enum_name(node.op);
        auto operands = std::vector<ReturnType> {};
        operands.reserve(node.operands.size());
        std::transform(node.operands.begin(), node.operands.end(),
                       std::back_inserter(operands), [&](const auto& operand) {
                           return visit(operand);
                       });

        return fmt::format("BoolOp[op={}, operands=[{}]]", op,
                           fmt::join(operands, ", "));
    }
    ReturnType operator()(const Lambda& node) const {
        auto params = std::vector<ReturnType> {};
        params.reserve(node.params.size());
        std::transform(node.params.begin(), node.params.end(),
                       std::back_inserter(params), [&](const auto& v) {
                           return visit(v);
                       });
        auto expr = visit(node.expr);

        return fmt::format("Lambda[params=[{}], expr=[{}]]",
                           fmt::join(params, ", "), expr);
    }
    ReturnType operator()(const Expression& node) const {
        return fmt::format("Expression[expr={}]", visit(node.expr));
    }
    ReturnType operator()(const AssignStatement& node) const {
        return fmt::format("AssignStatement[target={}, expr={}]",
                           visit(node.target), visit(node.expr));
    }
    ReturnType operator()(const LazyAssignStatement& node) const {
        return fmt::format("LazyAssignStatement[target={}, expr={}]",
                           visit(node.target), visit(node.expr));
    }
    ReturnType operator()(const AugAssignStatement& node) const {
        return fmt::format("AugAssignStatement[target={}, op={}, expr={}]",
                           visit(node.target), magic_enum::enum_name(node.op),
                           visit(node.expr));
    }
    ReturnType operator()(const ReturnStatement& node) const {
        if (node.expr.has_value()) {
            return fmt::format("ReturnStatement[expr={}]",
                               visit(node.expr.value()));
        } else {
            return fmt::format("ReturnStatement[]");
        }
    }
    ReturnType operator()(const StatementList& node) const {
        auto stmts = std::vector<ReturnType> {};
        stmts.reserve(node.stmts.size());
        std::transform(node.stmts.begin(), node.stmts.end(),
                       std::back_inserter(stmts), [&](const auto& v) {
                           return visit(v);
                       });

        return fmt::format("StatementList[stmts=[{}]]", fmt::join(stmts, ", "));
    }

    ReturnType operator()(const ExternFunctionDecl& node) const {
        (void)node;

        return fmt::format("ExternFunctionDecl[]");
    }
    ReturnType operator()(const FunctionDef& node) const {
        auto decorators = std::vector<ReturnType> {};
        decorators.reserve(node.decorators.size());
        std::transform(node.decorators.begin(), node.decorators.end(),
                       std::back_inserter(decorators), [&](const auto& v) {
                           return visit(v);
                       });
        auto name = visit(node.name);
        auto params = std::vector<ReturnType> {};
        params.reserve(node.params.size());
        std::transform(node.params.begin(), node.params.end(),
                       std::back_inserter(params), [&](const auto& v) {
                           return visit(v);
                       });
        auto body = visit(node.body);

        if (decorators.empty()) {
            return fmt::format("FunctionDef[name={}, params=[{}], body=[{}]]",
                               name, fmt::join(params, ", "), body);
        } else {
            return fmt::format(
                "FunctionDef[decorators=[{}], name={}, params=[{}], body=[{}]]",
                fmt::join(decorators, ", "), name, fmt::join(params, ", "),
                body);
        }
    }

    ReturnType operator()(const IfStatement& node) const {
        auto condition = visit(node.condition);
        auto body = visit(node.body);
        auto or_else = visit(node.or_else);

        return fmt::format(
            "IfStatement[condition=[{}], body=[{}], or_else=[{}]]", condition,
            body, or_else);
    }
    ReturnType operator()(const ForStatement& node) const {
        auto init = visit(node.init);
        auto condition = visit(node.condition);
        auto iter = visit(node.iter);
        auto body = visit(node.body);

        return fmt::format(
            "ForStatement[init=[{}], condition=[{}], iter=[{}], body=[{}]]",
            init, condition, iter, body);
    }
    ReturnType operator()(const RangeBasedForStatement& node) const {
        (void)node;

        return fmt::format("RangeBasedForStatement[]");
    }
    ReturnType operator()(const WhileStatement& node) const {
        auto condition = visit(node.condition);
        auto body = visit(node.body);

        return fmt::format("WhileStatement[condition=[{}], body=[{}]]",
                           condition, body);
    }

    ReturnType operator()(const Pass& node) const {
        (void)node;

        return fmt::format("Pass[]");
    }
    ReturnType operator()(const Break& node) const {
        (void)node;

        return fmt::format("Break[]");
    }
    ReturnType operator()(const Continue& node) const {
        (void)node;

        return fmt::format("Continue[]");
    }

    ReturnType operator()(const ImportPackage& node) const {
        return fmt::format("ImportPackage[path={}]", visit(node.path));
    }

    ReturnType operator()(const PackageName& node) const {
        return fmt::format("PackageName[path={}]", visit(node.path));
    }

    ReturnType operator()(const Entry& node) const {
        return fmt::format("Entry[package={}, node={}]", visit(node.package),
                           visit(node.node));
    }
};

}    // namespace expressions::ast

#endif
