//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

package system.algorithm;


@builtin
def make_heap(iterable) -> vector;

@builtin
def make_heap(iterable, compare) -> vector;

@builtin
def push_heap(iterable) -> vector;

@builtin
def push_heap(iterable, compare) -> vector;

@builtin
def pop_heap(iterable) -> vector;

@builtin
def pop_heap(iterable, compare) -> vector;

@builtin
def sort_heap(iterable) -> vector;

@builtin
def sort_heap(iterable, compare) -> vector;

@builtin
def is_heap(iterable) -> vector;

@builtin
def is_heap(iterable, compare) -> vector;
