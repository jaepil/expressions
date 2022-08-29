//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_PARSER_TRANSFORM_COMPARE_OP_TRANSFORMER_HPP__
#define __EXPRESSIONS_PARSER_TRANSFORM_COMPARE_OP_TRANSFORMER_HPP__

#include <expressions/parser/transform/recursive_node_transformer.hpp>


namespace expressions::parser {

class CompareOpTransformer
    : public RecursiveNodeTransformer<CompareOpTransformer> {
public:
    using RecursiveNodeTransformer::operator();

    ast::Value operator()(const ast::CompareOp& node) const {
        auto first = visit(node.first);
        if (node.rest.empty()) {
            return first;
        }

        auto rest = std::vector<ast::CompareOpOperand> {};
        rest.reserve(node.rest.size());
        for (const auto& n : node.rest) {
            rest.emplace_back(ast::CompareOpOperand {n.op, visit(n.operand)});
        }

        return ast::Value {ast::CompareOp {std::move(first), std::move(rest)}};
    }

    ast::Value operator()(const ast::UnaryOp& node) const {
        auto value = visit(node.operand);

        return ast::Value {ast::UnaryOp {node.op, std::move(value)}};
    }

    ast::Value operator()(const ast::BoolOp& node) const {
        auto operands = std::vector<ast::Value> {};
        operands.reserve(node.operands.size());
        for (const auto& operand : node.operands) {
            operands.emplace_back(visit(operand));
        }

        if (operands.size() == 1) {
            return ast::Value {std::move(operands[0])};
        }

        return ast::Value {ast::BoolOp {node.op, std::move(operands)}};
    }

    ast::Value operator()(const ast::StatementList& node) const {
        auto stmts = std::vector<ast::Value> {};
        stmts.reserve(node.stmts.size());
        for (const auto& stmt : node.stmts) {
            stmts.emplace_back(visit(stmt));
        }

        if (stmts.size() == 1) {
            return ast::Value {std::move(stmts[0])};
        }

        return ast::Value {ast::StatementList {std::move(stmts)}};
    }
};

}    // namespace expressions::parser

#endif
