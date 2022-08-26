#
# Expressions
#
# Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
#

add_subdirectory(parser)

add_library(expressions-src INTERFACE)
target_link_libraries(
    expressions-src INTERFACE
    $<TARGET_OBJECTS:expressions-parser>
)
