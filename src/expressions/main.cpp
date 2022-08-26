//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#include <fmt/format.h>

#include <cstdint>
#include <string_view>


int32_t execute_script(const std::string_view& input) {
    (void)input;

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
