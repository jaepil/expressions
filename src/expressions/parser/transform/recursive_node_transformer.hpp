//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_PARSER_TRANSFORM_RECURSIVE_NODE_TRANSFORMER_HPP__
#define __EXPRESSIONS_PARSER_TRANSFORM_RECURSIVE_NODE_TRANSFORMER_HPP__

#include <expressions/ast/ast_visitor.hpp>


namespace expressions::parser {

template<typename T>
class RecursiveNodeTransformer
    : public ast::RecursiveASTNodeVisitor<T, ast::Value> {
public:
    RecursiveNodeTransformer() = default;

    using ast::RecursiveASTNodeVisitor<T, ast::Value>::visit;
    using ast::RecursiveASTNodeVisitor<T, ast::Value>::operator();

    template<typename ValueType,
             std::enable_if_t<
                 boost::mp11::mp_contains<
                     ast::Value::types, std::remove_cvref_t<ValueType>>::value,
                 int> = 0>
    ast::Value transform(ValueType&& node) const {
        return visit(std::forward<ValueType>(node));
    }
    ast::Value transform(const ast::Value& node) const {
        return visit(node);
    }
};

}    // namespace expressions::parser

#endif
