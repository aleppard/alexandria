# Library

## Source Code

The library source code is written in a marked-up version of C++ using
the Modules Technical Specification (TS). The use of modules removes
the need to write header files. Since the source code is marked up,
the transpiler should also output C++ as a target. The "marking-up" is
done using template parameter names. A future version could utilise
the upcoming Concepts TS.

The use of the export keyword indicates that the variable or function is public.

    export function()

The "export" syntax is part of C++ Module TS. It is not standard C++
and clang's module TS support does not appear to be fully supported
by the clang toolchain. For now it is included by #define to an empty string.
Its purpose is to avoid having a single header and body file for the source C++.

    template<typename DOUBLE> ...

Use of DOUBLE template parameter will act differently depending on the
language features, e.g.

    template<typename DOUBLE> DOUBLE cos(DOUBLE x) {}
    template<typename FLOAT> FLOAT cos(FLOAT x) {}
    // template<typename LONG_DOUBLE> LONG_DOUBLE cos(LONG_DOUBLE x) {}
    // template<typename COMPLEX> COMPLEX cos(COMPLEX x) {}

type_declarations: no

Will produce a single function:

    def cos(x)

type_declarations: yes
functions: overloading: yes

Will produce two functions with the same name, one for each type:

    double cos(double x)
    float cos(float x) 

type_declarations: yes
functions: overloading: no

Will produce two functions with different names, one for each type:

    double cos(double x)
    float cosf(float x)

If a single C++ implementation can generate a float and a double
version, then this can be reflected by the template parameter name,
e.g.

    template<typename FLOAT_DOUBLE> FLOAT_DOUBLE cos(FLOAT_DOUBLE x)
