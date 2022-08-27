//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_PARSER_TRANSFORM_BOOL_OP_TRANSFORMER_HPP__
#define __EXPRESSIONS_PARSER_TRANSFORM_BOOL_OP_TRANSFORMER_HPP__

#include <expressions/parser/transform/recursive_node_transformer.hpp>

#include <vector>


namespace expressions::parser {

class BoolOpTransformer : public RecursiveNodeTransformer<BoolOpTransformer> {
public:
    BoolOpTransformer() = default;
    explicit BoolOpTransformer(ast::BoolOpType op_type) : op_type_(op_type) {
    }

    using RecursiveNodeTransformer::operator();

    ast::Value operator()(const ast::BoolOp& node) const {
        auto operands = std::vector<ast::Value> {};
        operands.reserve(node.operands.size());
        for (const auto& operand : node.operands) {
            operands.emplace_back(visit(operand));
        }

        if (operands.size() == 1) {
            return ast::Value {std::move(operands[0])};
        }

        auto op = node.op;
        if (op == ast::BoolOpType::kDefault) {
            op = op_type_;
        }

        return ast::Value {ast::BoolOp {op, std::move(operands)}};
    }

private:
    ast::BoolOpType op_type_ {ast::BoolOpType::kAnd};
};

}    // namespace expressions::parser

#endif
