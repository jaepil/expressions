//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_PARSER_PARSER_HPP__
#define __EXPRESSIONS_PARSER_PARSER_HPP__

#include <memory>
#include <string_view>


namespace expressions::ast {
struct Entry;
}

namespace expressions::parser {

class ExpressionsParser {
public:
    ExpressionsParser() = default;

    auto parse_to_ast(const std::string_view& input) const
        -> std::shared_ptr<ast::Entry>;

private:
    bool parse_to_tree_(const std::string_view& input, ast::Entry& output,
                        bool transform) const;
    bool transform_tree_(ast::Entry& tree) const;
};

}    // namespace expressions::parser

#endif
