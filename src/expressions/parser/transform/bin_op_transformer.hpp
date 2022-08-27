//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_PARSER_TRANSFORM_BIN_OP_TRANSFORMER_HPP__
#define __EXPRESSIONS_PARSER_TRANSFORM_BIN_OP_TRANSFORMER_HPP__

#include <expressions/parser/transform/recursive_node_transformer.hpp>


namespace expressions::parser {

class BinOpTransformer : public RecursiveNodeTransformer<BinOpTransformer> {
public:
    using RecursiveNodeTransformer::operator();

    ast::Value operator()(const ast::BinOpIntermediate& node) const {
        auto left = visit(node.first);
        for (const auto& operand : node.rest) {
            auto right = visit(operand.operand);
            left = ast::BinOp {std::move(left), operand.op, std::move(right)};
        }

        return left;
    }
};

}    // namespace expressions::parser

#endif
