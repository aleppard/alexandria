# Languages

The idea of these YAML files is to specify as many of the properties of each
target language as possible so that the transpiler can be as language agnostic as possible. This way the transpiler does not grow linearly with the number of target languages. It also simplifies the process of adding a new language. Many languages share similar (or the same approaches) in many ways, so this allows a single implementation for those shared approaches across languages.

It is unlikely that all language differences can be described simply in these files. So the transpiler will most likely still need some language specific code.

## Classes

## Functions

## Variables

## Operators

## Statements

## Types

types: none | optional | required

A value of 'none' means that types are not specified for variables or function parameters, e.g. for Python:

var count = 0;

A value of 'optional' means that types do not need to be specified for variables or function types, e.g. Rust.

A value of 'required' means that types must be specified (e.g. C, C++ etc).
