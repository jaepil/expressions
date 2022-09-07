//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

package org.expressions.examples.decorator;

import system.console;


def nop(f) {
    return f;
}

def trace(f) {
    def wrapper(args...) {
        result = f(args...);
        print(result);

        return result;
    }

    return wrapper;
}

@nop
@trace
def say_hello(name) {
    return "Hello, " + name;
}

say_hello("World!");
