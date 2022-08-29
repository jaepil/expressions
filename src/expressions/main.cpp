//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#include <expressions/interpreter/ast_interpreter.hpp>
#include <expressions/parser/parser.hpp>

#include <expressions/common/enumerate.hpp>
#include <expressions/common/visitor.hpp>

#include <fmt/format.h>

#include <cstdint>
#include <fstream>
#include <string>


int32_t execute_script(const std::string_view& input) {
    using namespace expressions;

    auto ifs = std::ifstream {input, std::ios::in};
    if (!ifs.is_open()) {
        return -1;
    }
    auto code = std::string {};
    std::getline(ifs, code, static_cast<char>(std::char_traits<char>::eof()));
    fmt::print("{}", code);

    auto parser = parser::ExpressionsParser {};
    auto tree = parser.parse_to_ast(code);

    auto interp = interpreter::ASTInterpreter {};
    auto result = interp.execute(*tree);

    auto v = SelfVisitableVisitor {
        [](auto&&, interpreter::Null) {
            return std::string {"null"};
        },
        [](auto&&, bool value) {
            return fmt::format("{}", value);
        },
        [](auto&&, int64_t value) {
            return fmt::format("{}", value);
        },
        [](auto&&, uint64_t value) {
            return fmt::format("{}", value);
        },
        [](auto&&, double value) {
            return fmt::format("{}", value);
        },
        [](auto&&, const interpreter::Name& value) {
            return value.value;
        },
        [](auto&&, const interpreter::String& value) {
            return fmt::format("\"{}\"", value.value);
        },
        [](auto&&, const interpreter::Date& value) {
            return fmt::format("{:04}-{:02}-{:02}", value.year, value.month,
                               value.day);
        },
        [](auto&&, const interpreter::DateRange& value) {
            return fmt::format("{:04}-{:02}-{:02}-{:04}-{:02}-{:02}",
                               value.begin.year, value.begin.month,
                               value.begin.day, value.end.year, value.end.month,
                               value.end.day);
        },
        [](auto&& self, const interpreter::Tuple<interpreter::BoxedValue>& c) {
            auto buffer = fmt::memory_buffer {};
            auto output = std::back_inserter(buffer);
            fmt::format_to(output, "(");
            for (const auto& [index, item] : enumerate(c)) {
                if (index > 0) {
                    fmt::format_to(output, ", ");
                }
                fmt::format_to(output, "{}", self.visit(item));
            }
            fmt::format_to(output, ")");

            return fmt::to_string(buffer);
        },
        [](auto&& self, const interpreter::Vector<interpreter::BoxedValue>& c) {
            auto buffer = fmt::memory_buffer {};
            auto output = std::back_inserter(buffer);
            fmt::format_to(output, "[");
            for (const auto& [index, item] : enumerate(c)) {
                if (index > 0) {
                    fmt::format_to(output, ", ");
                }
                fmt::format_to(output, "{}", self.visit(item));
            }
            fmt::format_to(output, "]");

            return fmt::to_string(buffer);
        },
        [](auto&& self, const interpreter::Set<interpreter::BoxedValue>& c) {
            auto buffer = fmt::memory_buffer {};
            auto output = std::back_inserter(buffer);
            fmt::format_to(output, "<<?");
            for (const auto& [index, item] : enumerate(c)) {
                if (index > 0) {
                    fmt::format_to(output, ", ");
                }
                fmt::format_to(output, "{}", self.visit(item));
            }
            fmt::format_to(output, "?>>");

            return fmt::to_string(buffer);
        },
        [](auto&&, const interpreter::Map<interpreter::BoxedValue,
                                          interpreter::BoxedValue>& c) {
            (void)c;
            return std::string {};
        },
        [](auto&&, const auto& value) {
            auto name
                = boost::typeindex::type_id<decltype(value)>().pretty_name();
            return fmt::format("{}", name);
        },
    };
    auto output = v.visit(result);
    fmt::print("{}\n", output);

    return 0;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fmt::print("Usage: expressions <filename>\n");
        return 1;
    }

    fmt::print("Executing {}...\n", argv[1]);
    auto result = execute_script(argv[1]);
    fmt::print("Result: {}\n", result);

    return 0;
}
