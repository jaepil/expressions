//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

package org.expressions.examples.hello;


def make_string(name) {
    return "Hello, " + name;
}

say_hello = make_string;

print_hello = (name) => print(say_hello(name));

print_hello("World!")
