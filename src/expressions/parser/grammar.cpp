//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#include <expressions/parser/grammar.hpp>

#include <functional>
#include <string_view>


namespace expressions::parser {

namespace x3 = boost::spirit::x3;

using skipper_type = decltype(skipper);
using iterator_type = std::string_view::const_iterator;
using error_handler_type = x3::error_handler<iterator_type>;
using phrase_context_type = x3::phrase_parse_context<skipper_type>::type;
using context_type = x3::context<x3::error_handler_tag,
                                 std::reference_wrapper<error_handler_type>,
                                 phrase_context_type>;

BOOST_SPIRIT_INSTANTIATE(entry_type, iterator_type, context_type)

const entry_type& get_entry_rule() {
    return entry;
}

}    // namespace expressions::parser
