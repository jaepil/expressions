//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#include <expressions/parser/grammar.hpp>

#include <string>


namespace expressions::parser {

namespace x3 = boost::spirit::x3;

using iterator_type = std::string::const_iterator;
using context_type = x3::phrase_parse_context<x3::unicode::space_type>::type;

BOOST_SPIRIT_INSTANTIATE(entry_type, iterator_type, context_type)

const entry_type& get_entry_rule() {
    return entry;
}

}    // namespace expressions::parser
