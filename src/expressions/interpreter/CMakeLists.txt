#
# Expressions
#
# Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
#

set(SOURCE_FILES
    ast_interpreter.cpp
)

add_library(expressions-interpreter OBJECT ${SOURCE_FILES})
