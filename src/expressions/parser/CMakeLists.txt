#
# Expressions
#
# Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
#

set(SOURCE_FILES
    grammar.cpp
    parser.cpp
)

add_library(expressions-parser OBJECT ${SOURCE_FILES})
