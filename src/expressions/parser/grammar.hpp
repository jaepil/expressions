//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_PARSER_GRAMMAR_HPP__
#define __EXPRESSIONS_PARSER_GRAMMAR_HPP__

#include <expressions/parser/grammar_def.hpp>


namespace expressions::parser {

BOOST_SPIRIT_DECLARE(entry_type)

const entry_type& get_entry_rule();

}    // namespace expressions::parser

#endif
