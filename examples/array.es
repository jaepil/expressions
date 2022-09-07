//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

package org.expressions.examples.array;

import system.console;


def find_in_array(iterable, cmp) {
    output = []
    for (i = 0; i < len(iterable); i += 1) {
        if (cmp(iterable[i])) {
            output += [iterable[i]];
        }
    }

    return output
}

data = [0, 1, 1.5, 2, 2.5, 3, 4, 5];
print("Input:", data);

output = find_in_array(data, (e) => return e < 2);
print("Output where e < 2:", output);
