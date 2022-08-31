//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#include <expressions/parser/parser.hpp>

#include <expressions/ast/ast.hpp>
#include <expressions/parser/grammar.hpp>

#include <expressions/parser/transform/bin_op_transformer.hpp>
#include <expressions/parser/transform/bool_op_transformer.hpp>
#include <expressions/parser/transform/compare_op_transformer.hpp>

#include <expressions/support/boost/spirit.hpp>

#include <functional>
#include <iostream>


namespace expressions::parser {

auto ExpressionsParser::parse_to_ast(const std::string_view& input) const
    -> std::shared_ptr<ast::Entry> {
    auto tree = ast::Entry {};
    if (!parse_to_tree_(input, tree, true)) {
        return nullptr;
    }

    return std::make_shared<ast::Entry>(tree);
}

bool ExpressionsParser::parse_to_tree_(const std::string_view& input,
                                       ast::Entry& output,
                                       bool transform) const {
    auto begin = std::cbegin(input);
    auto end = std::cend(input);
    auto on_error = x3::error_handler<std::string_view::const_iterator> {
        begin,
        end,
        std::cerr,
    };

    // clang-format off
    const auto parser
        = x3::with<x3::error_handler_tag>(std::ref(on_error))[
            get_entry_rule()
        ]
        ;
    // clang-format on

    auto tree = ast::Entry {};
    auto result = x3::phrase_parse(begin, end, parser, skipper, tree);
    if (!result) {
        return false;
    }

    if (transform && !transform_tree_(tree)) {
        return false;
    }

    output = std::move(tree);

    return true;
}

bool ExpressionsParser::transform_tree_(ast::Entry& tree) const {
    auto new_tree = BinOpTransformer {}.transform(tree);
    new_tree = CompareOpTransformer {}.transform(new_tree);
    new_tree = BoolOpTransformer {}.transform(new_tree);
    if (!ast::holds_alternative<ast::Entry>(new_tree)) {
        return false;
    }

    tree = ast::get<ast::Entry>(new_tree);

    return true;
}

}    // namespace expressions::parser
