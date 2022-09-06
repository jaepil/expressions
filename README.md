# Expressions

## Introduction

Expressions is a simple script language with the Just-In-Time compilation. It is designed to implement high-performance, customizable ad-hoc components at runtime, such as ranking and sorting functions in database-like systems.

The JIT compiled code will work on x86-64 and AArch64 architectures, including Apple Silicon.

This is a personal and experimental project for implementing proof-of-concept of my ideas. The progress won't be fast since I will use my spare time on this project.

## Current Status

Currently, I'm working on the AST interpreter to determine the minimum viable language features. The code quality is not good as I'm using a quick and dirty approach. I will clean that mess up later.

The next step will be creating a minimal runtime library. It will provide minimal functionalities such as `print()`.

After that, I will implement a bytecode generator and stack-based virtual machine. We can consider this step as a preparation for designing the JIT compilation process.
