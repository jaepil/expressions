//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#ifndef __EXPRESSIONS_PARSER_GRAMMAR_DEF_HPP__
#define __EXPRESSIONS_PARSER_GRAMMAR_DEF_HPP__

#include <expressions/ast/ast.hpp>
#ifdef BOOST_SPIRIT_X3_DEBUG
#include <expressions/ast/ast_dump.hpp>
#endif

#include <expressions/support/boost/spirit.hpp>

#include <fmt/format.h>

#include <string>
#include <vector>


// NOTE: Boost Spirit is a complex framework, and it pushes compiler to the
// limit. Also, error messages from compilers are complete garbage most of the
// time. If you have trouble with dirty compile errors from Boost Spirit, these
// links are might be helpful.
// https://stackoverflow.com/questions/50335199/can-spirit-x3-work-with-boost-fusion-adapt-adt
// https://stackoverflow.com/search?q=user:85371+boost-spirit
// https://github.com/boostorg/spirit/issues/463

namespace expressions::parser {

namespace x3 = boost::spirit::x3;

// type declaration
using entry_type = x3::rule<struct entry_class, ast::Entry>;
using package_name_type = x3::rule<struct package_name_class, ast::PackageName>;
using statement_type = x3::rule<struct statement_class, ast::Value>;

using compound_statement_type
    = x3::rule<struct compound_statement_class, ast::Value>;
using if_statement_type = x3::rule<struct if_statement_class, ast::IfStatement>;
using for_statement_type = x3::rule<struct for_statement_class, ast::Value>;
using for_init_type = x3::rule<struct for_init_class, ast::AssignStatement>;
using for_condition_type = x3::rule<struct for_condition_class, ast::Value>;
using for_iteration_type
    = x3::rule<struct for_iteration_class, ast::AugAssignStatement>;
using classic_for_statement_type
    = x3::rule<struct classic_for_statement_class, ast::ForStatement>;
using for_target_type = x3::rule<struct for_target_class, ast::List>;
using for_targets_type = x3::rule<struct for_targets_class, ast::Value>;
using range_based_for_statement_type
    = x3::rule<struct for_statement_class, ast::RangeBasedForStatement>;
using while_statement_type
    = x3::rule<struct while_statement_class, ast::WhileStatement>;

using simple_statement_type
    = x3::rule<struct simple_statement_class, ast::Value>;
using assign_statement_type
    = x3::rule<struct assign_statement_class, ast::AssignStatement>;
using lazy_assign_statement_type
    = x3::rule<struct lazy_assign_statement_class, ast::LazyAssignStatement>;
using aug_assign_statement_type
    = x3::rule<struct aug_assign_statement_class, ast::AugAssignStatement>;

using return_statement_type
    = x3::rule<struct return_statement_class, ast::ReturnStatement>;
using pass_statement_type = x3::rule<struct pass_statement_class, ast::Pass>;
using break_statement_type = x3::rule<struct break_statement_class, ast::Break>;
using continue_statement_type
    = x3::rule<struct continue_statement_class, ast::Continue>;

using statement_list_type
    = x3::rule<struct statement_list_class, ast::StatementList>;

using alias_target_type = x3::rule<struct alias_target_class, ast::List>;
using alias_targets_type = x3::rule<struct alias_targets_class, ast::Value>;
using aliased_expression_type
    = x3::rule<struct aliased_expression_class, ast::AliasedExpression>;
using expression_type = x3::rule<struct expression_class, ast::Value>;
using lambda_expr_type = x3::rule<struct lambda_expr_class, ast::Lambda>;

using bool_expr_type = x3::rule<struct bool_expr_class, ast::BoolOp>;
using bool_expr_or_type = x3::rule<struct bool_expr_or_class, ast::BoolOp>;
using bool_expr_and_type = x3::rule<struct bool_expr_and_class, ast::BoolOp>;
using unary_expr_type = x3::rule<struct unary_expr_class, ast::Value>;

using compare_ops_type = x3::rule<struct compare_ops_class, ast::CompareOpType>;
using compare_op_expr_type = x3::rule<struct compare_op_expr_class, ast::Value>;
using bin_op_expr_type = x3::rule<struct bin_op_expr_class, ast::Value>;
using bin_op_additive_expr_type
    = x3::rule<struct bin_op_additive_expr_class, ast::Value>;
using bin_op_multiplicative_expr_type
    = x3::rule<struct bin_op_multiplicative_expr_class, ast::Value>;
using bin_op_exponential_expr_type
    = x3::rule<struct bin_op_exponential_expr_class, ast::Value>;

using primary_type = x3::rule<struct primary_class, ast::Value>;
using call_type = x3::rule<struct call_class, ast::Call>;
using argument_type = x3::rule<struct argument_class, ast::Argument>;
using keyword_argument_type
    = x3::rule<struct keyword_argument_class, ast::KeywordArgument>;
using argument_list_type
    = x3::rule<struct argument_list_class, std::vector<ast::Value>>;

using named_expr_type = x3::rule<struct named_arg_class, ast::NamedExpression>;
using named_expr_list_type
    = x3::rule<struct named_arg_list_class, ast::NamedExpressionList>;
using atom_type = x3::rule<struct atom_class, ast::Value>;
using numbers_type = x3::rule<struct numbers_class, ast::Value>;
using id_type = x3::rule<struct id_class, ast::Name>;
using quoted_string_type
    = x3::rule<struct quoted_string_class, ast::QuotedString>;

using sequence_type = x3::rule<struct sequence_class, ast::Value>;
using group_type = x3::rule<struct group_class, ast::Value>;
using tuple_type = x3::rule<struct tuple_class, ast::Tuple>;
using list_type = x3::rule<struct list_class, ast::List>;
using dict_type = x3::rule<struct dict_class, ast::Dict>;
using dict_item_type = x3::rule<struct dict_item_class, ast::DictItem>;
using set_type = x3::rule<struct set_class, ast::Set>;

// type definition
static const entry_type entry {"entry"};
static const package_name_type package_name {"package_name"};
static const statement_type statement {"statement"};

static const compound_statement_type compound_statement {"compound_statement"};
static const if_statement_type if_statement {"if_statement"};
static const for_statement_type for_statement {"for_statement"};
static const for_init_type for_init {"for_init"};
static const for_condition_type for_condition {"for_condition"};
static const for_iteration_type for_iteration {"for_iteration"};
static const classic_for_statement_type classic_for_statement {
    "classic_for_statement"};
static const for_target_type for_target {"for_target"};
static const for_targets_type for_targets {"for_targets"};
static const range_based_for_statement_type range_based_for_statement {
    "range_based_for_statement"};
static const while_statement_type while_statement {"while_statement"};

static const simple_statement_type simple_statement {"simple_statement"};
static const assign_statement_type assign_statement {"assign_statement"};
static const lazy_assign_statement_type lazy_assign_statement {
    "lazy_assign_statement"};
static const aug_assign_statement_type aug_assign_statement {
    "aug_assign_statement"};

static const return_statement_type return_statement {"return_statement"};
static const pass_statement_type pass_statement {"pass_statement"};
static const break_statement_type break_statement {"break_statement"};
static const continue_statement_type continue_statement {"continue_statement"};

static const statement_list_type statement_list {"statement_list"};

static const alias_target_type alias_target {"alias_target"};
static const alias_targets_type alias_targets {"alias_targets"};
static const aliased_expression_type aliased_expression {"aliased_expression"};
static const expression_type expression {"expression"};
static const lambda_expr_type lambda_expr {"lambda_expr"};

static const bool_expr_type bool_expr {"bool_expr"};
static const bool_expr_or_type bool_expr_or {"bool_expr_or"};
static const bool_expr_and_type bool_expr_and {"bool_expr_and"};
static const unary_expr_type unary_expr {"unary_expr"};

static const compare_ops_type compare_ops {"compare_ops"};
static const compare_op_expr_type compare_op_expr {"compare_op_expr"};
static const bin_op_expr_type bin_op_expr {"bin_op_expr"};
static const bin_op_additive_expr_type bin_op_additive_expr {
    "bin_op_additive_expr"};
static const bin_op_multiplicative_expr_type bin_op_multiplicative_expr {
    "bin_op_multiplicative_expr"};
static const bin_op_exponential_expr_type bin_op_exponential_expr {
    "bin_op_exponential_expr"};

static const primary_type primary {"primary"};
static const call_type call {"call"};
static const argument_type argument {"argument"};
static const keyword_argument_type keyword_argument {"keyword_argument"};
static const argument_list_type argument_list {"argument_list"};

static const named_expr_type named_expression {"named_expression"};
static const named_expr_list_type named_expression_list {
    "named_expression_list"};
static const atom_type atom {"atom"};
static const numbers_type numbers {"numbers"};
static const id_type id {"id"};
static const quoted_string_type quoted_string {"quoted_string"};

static const sequence_type sequence {"sequence"};
static const group_type group {"group"};
static const tuple_type tuple {"tuple"};
static const list_type list {"list"};
static const dict_type dict {"dict"};
static const dict_item_type dict_item {"dict_item"};
static const set_type set {"set"};

// clang-format off
// grammar rule construction
static const auto unary_op = x3::symbols<ast::BoolOpType> {
    {"!", ast::BoolOpType::kNot},
    {"+", ast::BoolOpType::kPlus},
    {"-", ast::BoolOpType::kMinus},
};
static const auto unary_op_not = x3::symbols<ast::BoolOpType> {
    {"not", ast::BoolOpType::kNot},
};

static const auto bool_op_and = x3::symbols<ast::BoolOpType> {
    {"and", ast::BoolOpType::kAnd},
};

static const auto bool_op_or = x3::symbols<ast::BoolOpType> {
    {"or", ast::BoolOpType::kOr},
};

static const auto compare_op = x3::symbols<ast::CompareOpType> {
    {"==", ast::CompareOpType::kEQ},
    {"!=", ast::CompareOpType::kNEQ},

    {"<", ast::CompareOpType::kLT},
    {"<=", ast::CompareOpType::kLTE},

    {">", ast::CompareOpType::kGT},
    {">=", ast::CompareOpType::kGTE},
};

static const auto additive_op = x3::symbols<ast::BinOpType> {
    {"+", ast::BinOpType::kAdd},
    {"-", ast::BinOpType::kSub},
};

static const auto multiplicative_op = x3::symbols<ast::BinOpType> {
    {"*", ast::BinOpType::kMult},
    {"/", ast::BinOpType::kTrueDiv},
    {"//", ast::BinOpType::kFloorDiv},
    {"%", ast::BinOpType::kMod},
};

static const auto exponential_op = x3::symbols<ast::BinOpType> {
    {"**", ast::BinOpType::kPow},
};

static const auto alias_op = x3::symbols<ast::AliasType> {
    {"as", ast::AliasType::kAs},
};

static const auto reserved = x3::symbols<std::string_view> {
    "package",

    "import",
    "from",

    "if",
    "else",

    "for",
    "do",
    "while",

    "pass",
    "break",
    "continue",
    "return",

    "async",
    "await",
    "yield",

    "in",
    "not",

    "as",
};

// + - && || ! ( ) { } [ ] ^ " ~ * ? : \ / , ;
static const auto special_chars
    = x3::char_(R"(+-!(){}[]^"~*?:\/,;=<> )")
    // | x3::char_("*?")
    | x3::lit("&&")
    | x3::lit("||")
    | x3::lit("=>")
    ;

static const auto escaped_chars
    = "\\b" >> x3::attr('\b')
    | "\\t" >> x3::attr('\t')
    | "\\n" >> x3::attr('\n')
    | "\\f" >> x3::attr('\f')
    | "\\r" >> x3::attr('\r')
    | "\\v" >> x3::attr('\v')
    | "\\"  >> x3::int_parser<char, 8, 1, 3> {}
    | "\\x" >> x3::int_parser<char, 16, 2, 2> {}
    | "\\"  >> x3::char_
    ;

static const auto bin_ops = exponential_op | additive_op | multiplicative_op;

static const auto keywords
    = unary_op | bin_ops
    | x3::distinct(unary_op_not)
    | x3::distinct(bool_op_and | bool_op_or)
    | x3::distinct(alias_op)
    | x3::distinct(reserved)
    ;

static const auto entry_def
    = package_name >> -statement_list
    ;

static const auto package_name_def
    = "package" > (id % x3::lit('.')) > *x3::lit(';')
    ;

static const auto compound_stmt_lookahead
    = x3::lit("if")
    | x3::lit("for")
    | x3::lit("while")
    ;

static const auto statement_def
    = &compound_stmt_lookahead >> compound_statement
    | simple_statement
    ;

static const auto statement_list_def
    = statement % *x3::lit(';')
    ;

static const auto compound_statement_def
    = &x3::lit("if") >> if_statement
    | &x3::lit("for") >> for_statement
    | &x3::lit("while") >> while_statement
    ;

static const auto statement_body
    = x3::confix('{', '}')[statement_list]
    | statement
    ;

static const auto if_statement_def
    = "if" >> x3::confix('(', ')')[expression] >> statement_body
        >> -(x3::distinct("else") >> statement_body)
    ;

static const auto for_statement_def
    = classic_for_statement
    | range_based_for_statement
    ;

static const auto for_init_def
    = id >> '=' >> expression
    ;

static const auto for_condition_def
    = expression
    ;

static const auto for_iteration_def
    = id >> x3::lexeme[bin_ops >> '='] >> expression
    ;

static const auto classic_for_statement_def
    = "for" >> x3::confix('(', ')')[
        for_init >> ';' >> for_condition >> ';' >> for_iteration
    ] >> statement_body >> -(x3::distinct("else") >> statement_body)
    ;

static const auto for_target_def
    = id % ','
    | x3::confix('(', ')')[id % ',']
    | x3::confix('[', ']')[id % ',']
    ;

static const auto for_targets_def
    = id >> !x3::lit(',')
    | for_target
    ;

static const auto range_based_for_statement_def
    = "for" >> x3::confix('(', ')')[
        for_targets >> ':' >> expression
    ] >> statement_body >> -(x3::distinct("else") >> statement_body)
    ;

static const auto while_statement_def
    = "while" >> x3::confix('(', ')')[expression] >> statement_body
        >> -(x3::distinct("else") >> statement_body)
    ;

static const auto simple_statement_def
    = assign_statement
    | lazy_assign_statement
    | aug_assign_statement
    | expression >> !x3::distinct(alias_op)
    | return_statement
    | pass_statement
    | break_statement
    | continue_statement
    ;

static const auto assign_statement_def
    = id >> '=' >> expression >> *x3::lit(';')
    ;

static const auto lazy_assign_statement_def
    = id >> ":=" >> expression >> *x3::lit(';')
    ;

static const auto aug_assign_statement_def
    = id >> x3::lexeme[bin_ops >> '='] >> expression >> *x3::lit(';')
    ;

static const auto return_statement_def
    = x3::distinct("return") >> -expression >> *x3::lit(';')
    ;

static const auto pass_statement_def
    = x3::distinct("pass") >> *x3::lit(';')
    ;

static const auto break_statement_def
    = x3::distinct("break") >> *x3::lit(';')
    ;

static const auto continue_statement_def
    = x3::distinct("continue") >> *x3::lit(';')
    ;

static const auto sequence_lookahead
    = x3::lit('(')
    | x3::lit('[')
    | x3::lit('{')
    ;

static const auto lambda_body
    = &sequence_lookahead >> sequence
    | statement_body
    ;

static const auto lambda_expr_def
    = '(' >> -argument_list >> ')' >> "=>" >> lambda_body
    ;

static const auto expression_def
    = &x3::lit('(') >> lambda_expr
    | bool_expr
    ;

static const auto bool_expr_def
    = bool_expr_or
    ;

static const auto bool_expr_or_def
    = x3::attr(ast::BoolOpType::kOr) >> (
        bool_expr_and % bool_op_or
    )
    ;

static const auto bool_expr_and_def
    = x3::attr(ast::BoolOpType::kAnd) >> (
        compare_op_expr % bool_op_and
    )
    ;

static const auto compare_ops_def
    = compare_op
    | x3::distinct("in") >> x3::attr(ast::CompareOpType::kIn)
    | x3::distinct("not") >> x3::distinct("in")
        >> x3::attr(ast::CompareOpType::kNotIn)
    ;

static const auto compare_op_expr_def
    = bin_op_expr >> !x3::distinct(alias_op) >> *(compare_ops > bin_op_expr)
    | bin_op_expr >> !x3::distinct(alias_op)
    | aliased_expression
    ;

static const auto alias_target_def
    = x3::confix('(', ')')[(quoted_string | id) % ',']
    | x3::confix('[', ']')[(quoted_string | id) % ',']
    ;

static const auto alias_targets_def
    = &(x3::lit('(') | x3::lit('[')) >> alias_target
    | quoted_string
    | id
    ;

static const auto aliased_expression_def
    = bin_op_expr >> x3::distinct(alias_op) >> atom
    ;

static const auto bin_op_expr_def
    = bin_op_additive_expr
    ;

static const auto bin_op_additive_expr_def
    = bin_op_multiplicative_expr >> *(additive_op > bin_op_multiplicative_expr)
    | bin_op_multiplicative_expr
    ;

static const auto bin_op_multiplicative_expr_def
    = bin_op_exponential_expr
        >> *(
            (!exponential_op >> multiplicative_op)
            > bin_op_exponential_expr
        )
    | bin_op_exponential_expr
    ;

static const auto bin_op_exponential_expr_def
    = unary_expr >> !exponential_op
    | unary_expr >> *(exponential_op > unary_expr)
    ;

static const auto unary_expr_def
    = (
        // not a
        &unary_op_not >> (unary_op_not > primary)
    ) | (
        // +a, -a, !a, #a
        &unary_op >> (
            x3::lexeme[
                unary_op >> !x3::unicode::space
            ] > primary
        )
    ) | primary
    ;

static const auto primary_def
    = &(id >> '(') >> call
    | &(unary_op | unary_op_not) >> unary_expr
    | atom
    ;

static const auto argument_def
    = (&x3::lit('(') >> lambda_expr) | unary_expr
    ;

static const auto keyword_argument_def
    = id >> '=' >> (
        (&x3::lit('(') >> lambda_expr)
        | unary_expr
    )
    ;

static const auto argument_list_def
    = ((keyword_argument | argument) % ',') >> -x3::lit(',') >> &x3::lit(')')
    ;

static const auto call_def
    = id >> x3::confix('(', ')')[-argument_list] >> !x3::lit("=>")
    ;

static const auto named_expression_def
    = id >> '=' >> unary_expr
    ;

static const auto named_expression_list_def
    = x3::confix('{', '}')[named_expression % ',']
    ;

static const auto numbers_def
    = x3::distinct(
        x3::strict_double
        | x3::int64
        | x3::uint64
    )
    ;

static const auto null
    = x3::distinct("null") >> x3::attr(ast::Null {})
    ;

static const auto boolean
    = x3::distinct("true") >> x3::attr(ast::Bool {true})
    | x3::distinct("false") >> x3::attr(ast::Bool {false})
    ;

static const auto verify_date = [](auto& ctx) {
    namespace fusion = boost::fusion;

    auto& attr = x3::_attr(ctx);
    auto year = fusion::at_c<0>(attr);
    auto month = fusion::at_c<1>(attr);
    auto day = fusion::at_c<2>(attr);

    if ((year >= 1900 && year <= 2100)
            && (month >= 1 && month <= 12)
            && (day >= 1 && day <= 31)) {
        auto& val = x3::_val(ctx);
        x3::traits::move_to(attr, val);
        x3::_pass(ctx) = true;
    } else {
        x3::_pass(ctx) = false;
    }
};

static const auto date_lookahead
    = x3::lexeme[
        x3::ranged_int_parser<int16_t, 10, 4, 4> {1900, 2100} >> x3::lit('-')
    ]
    ;

static const auto year_range_lookahead
    = x3::lexeme[
        x3::ranged_int_parser<int16_t, 10, 4, 4> {1900, 2100}
        >> x3::lit('-')
        >> x3::ranged_int_parser<int16_t, 10, 4, 4> {1900, 2100}
    ]
    ;

static const auto date
    = x3::as<ast::Date>[
        x3::lexeme[
            x3::int_parser<int16_t, 10, 4, 4> {}
            >> x3::lit('-') >> x3::int_parser<int8_t, 10, 1, 2> {}
            >> x3::lit('-') >> x3::int_parser<int8_t, 10, 1, 2> {}
        ][verify_date]
    ]
    ;

static const auto date_single
    = x3::lexeme[date >> !x3::lit('-')]
    ;

static const auto date_range
    = &x3::lexeme[date >> x3::lit('-')]
        >> x3::as<ast::DateRange>[
            x3::lexeme[
                date >> x3::lit('-') >> date
            ]
        ]
    | &year_range_lookahead
        >> x3::as<ast::DateRange>[
            x3::lexeme[
                x3::as<ast::Date>[
                    x3::int_parser<int16_t, 10, 4, 4> {}
                    >> x3::attr(1)
                    >> x3::attr(1)
                ]
                >> x3::lit('-')
                >> x3::as<ast::Date>[
                    x3::int_parser<int16_t, 10, 4, 4> {}
                    >> x3::attr(12)
                    >> x3::attr(31)
                ]
            ]
        ]
    ;

static const auto sequence_def
    = (tuple | group)
    | list
    | (dict | set)
    ;

static const auto number_lookahead
    = x3::strict_double
    | x3::distinct(x3::lit('0'))
    | !x3::lit('0')
    ;

static const auto atom_def
    = !keywords >> (
        (&date_lookahead >> date_single)
        | (&date_lookahead >> date_range)
        | id
        | null
        | boolean
        | (&number_lookahead >> numbers)
        | named_expression_list
        | quoted_string
        | (&sequence_lookahead >> sequence)
    )
    ;

static const auto id_def
    = x3::as<std::string>[
        x3::lexeme[
            (
                (x3::unicode::alpha | x3::char_('_') | x3::char_('.'))
                    >> *(x3::unicode::alnum | x3::char_('_') | x3::char_('.'))
            )
        ]
    ]
    ;

static const auto quoted_string_def
    = x3::lexeme[
        '"' > *(
            (escaped_chars | x3::char_) - x3::char_('"') - x3::eol
        ) > '"'
    ]
    ;

static const auto group_def
    = x3::confix('(', ')')[expression >> !x3::char_(',')]
    ;

static const auto tuple_def
    = x3::confix('(', ')')[x3::eps]
    | x3::confix('(', ')')[
        +(expression >> ',') >> -expression
    ]
    ;

static const auto list_def
    = x3::confix('[', ']')[x3::eps]
    | x3::confix('[', ']')[
        (expression % ',') >> -x3::lit(',')
    ]
    ;

static const auto dict_def
    = x3::confix('{', '}')[x3::eps]
    | x3::confix('{', '}')[
        (dict_item % ',') >> -x3::lit(',')
    ]
    ;

static const auto dict_item_def
    = atom >> x3::lit(":") >> expression
    | atom >> x3::lit("=>") >> expression
    ;

static const auto set_def
    = x3::confix('{', '}')[
        (expression % ',') >> -x3::lit(',')
    ]
    ;

static const auto skipper
    = x3::unicode::space
    | (!x3::lit("//=") >> "//" >> x3::seek[x3::eol | x3::eoi])
    ;
// clang-format on

BOOST_SPIRIT_DEFINE(entry, package_name, compound_statement, simple_statement,
                    if_statement, for_statement, for_init, for_condition,
                    for_iteration, classic_for_statement, for_target,
                    for_targets, range_based_for_statement, while_statement,
                    statement, assign_statement, lazy_assign_statement,
                    aug_assign_statement, return_statement, pass_statement,
                    break_statement, continue_statement, statement_list,
                    alias_target, alias_targets, aliased_expression, expression,
                    lambda_expr, bool_expr, bool_expr_or, bool_expr_and,
                    unary_expr, compare_ops, compare_op_expr, bin_op_expr,
                    bin_op_additive_expr, bin_op_multiplicative_expr,
                    bin_op_exponential_expr, primary, call, argument,
                    keyword_argument, argument_list, named_expression,
                    named_expression_list, atom, numbers, id, quoted_string,
                    sequence, group, tuple, list, dict, dict_item, set)

struct error_handler {
    template<typename Iterator, typename Exception, typename Context>
    x3::error_handler_result on_error(const Iterator& first,
                                      const Iterator& last, const Exception& e,
                                      const Context& context) {
        (void)first;
        (void)last;
        (void)context;

        auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
        auto message = "Error! Expecting: " + e.which() + " here:";
        error_handler(e.where(), message);

        return x3::error_handler_result::fail;

        // auto message = fmt::format("Error: expecting: {}, here: \"{}\"",
        //                            e.which(), std::string {e.where(), last});
        // fmt::print("{}\n", message);

        // return x3::error_handler_result::fail;
    }
};

struct entry_class : error_handler, x3::annotate_on_success {};

}    // namespace expressions::parser

#endif
