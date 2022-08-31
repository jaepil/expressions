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
#include <fmt/std.h>

#include <cstdint>
#include <fstream>
#include <string>
#include <variant>


using ExitValueType = std::variant<std::monostate, bool, int32_t, int64_t,
                                   uint32_t, uint64_t, double, std::string>;

auto execute_script(const std::string_view& input) -> ExitValueType {
    using namespace expressions;

    auto ifs = std::ifstream {input, std::ios::in};
    if (!ifs.is_open()) {
        return -1;
    }
    auto code = std::string {};
    std::getline(ifs, code, static_cast<char>(std::char_traits<char>::eof()));

    auto parser = parser::ExpressionsParser {};
    auto tree = parser.parse_to_ast(code);
    if (!tree) {
        return 1;
    }

    auto interp = interpreter::ASTInterpreter {};
    auto result = interp.execute(*tree);

    auto v = SelfVisitableVisitor {
        [](auto&&, interpreter::Null) -> ExitValueType {
            return std::monostate {};
        },
        [](auto&&, bool value) -> ExitValueType {
            return value;
        },
        [](auto&&, int64_t value) -> ExitValueType {
            return value;
        },
        [](auto&&, uint64_t value) -> ExitValueType {
            return value;
        },
        [](auto&&, double value) -> ExitValueType {
            return value;
        },
        [](auto&&, const interpreter::Name& value) -> ExitValueType {
            return value.value;
        },
        [](auto&&, const interpreter::String& value) -> ExitValueType {
            return fmt::format("\"{}\"", value.value);
        },
        [](auto&&, const interpreter::Date& value) -> ExitValueType {
            return fmt::format("{:04}-{:02}-{:02}", value.year, value.month,
                               value.day);
        },
        [](auto&&, const interpreter::DateRange& value) -> ExitValueType {
            return fmt::format("{:04}-{:02}-{:02}-{:04}-{:02}-{:02}",
                               value.begin.year, value.begin.month,
                               value.begin.day, value.end.year, value.end.month,
                               value.end.day);
        },
        [](auto&& self, const interpreter::Tuple<interpreter::BoxedValue>& c)
            -> ExitValueType {
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
        [](auto&& self, const interpreter::Vector<interpreter::BoxedValue>& c)
            -> ExitValueType {
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
        [](auto&& self, const interpreter::Set<interpreter::BoxedValue>& c)
            -> ExitValueType {
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
                                          interpreter::BoxedValue>& c)
            -> ExitValueType {
            (void)c;
            return std::string {};
        },
        [](auto&&, const auto& value) -> ExitValueType {
            auto name
                = boost::typeindex::type_id<decltype(value)>().pretty_name();
            return fmt::format("{}", name);
        },
    };

    auto output = v.visit(result);

    return output;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fmt::print("Usage: expressions <filename>\n");
        return 1;
    }

    fmt::print("Executing {}...\n\n", argv[1]);
    auto result = execute_script(argv[1]);
    fmt::print("\n{} has exited with code {}.\n", argv[1], result);

    return 0;
}
