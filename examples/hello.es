//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

package org.expressions.examples.hello;

import system.console;


// Define a function.
def make_string(name) {
    return "Hello, " + name;
}

// Define a variable pointing to a function.
say_hello = make_string;

// Define a variable from a lambda expression.
print_hello = (name) => print(say_hello(name));

// Call a lambda expression.
print_hello("World!")
