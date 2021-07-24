# Project Alexandria

## Background

As time goes on more and more [programming languages are created](https://en.wikipedia.org/wiki/Timeline_of_programming_languages). Each of these programming languages comes with its own standard library. While each standard library shares similarities with other programming languages their interfaces are almost always different and sometimes significantly so. New programmers to each language must learn not only the programming language itself, but also its standard library, and its ecosystem of popular libraries that supplement missing features in the standard library.

Language developers, unsurprisingly, often prioritise the development of the language itself and the standard library is often a secondary concern. Standard libraries are often incomplete and often have poor design choices. For example designing date and time libraries is a complicated problem in of itself and re-designing them for each language is not only unnecessary but often introduces eccentricities and annoyances which increases the burden on programmers who often struggle to work out how to do basic operations. Familiarity with one date and time library in another language often does not translate. Also standard library functionality often stagnant.

Language developers leverage users and the open source community to extend their standard libraries. This is pragmatic but problematic for multiple reasons. Firstly, because it introduces the problem of having to choose which third party library to use, it adds overhead integrating the library into the project and along with keeping that library and its dependencies up to date as well as dealing with cross-dependency issues. It's not uncommon for a project to bring in multiple different implementations of a library (e.g. JSON) simply because different dependencies have selected different implementations. This also adds the issue where each language needs its own implementation of functionality and often has multiple external libraries implementing it. The website json.org lists over 150 implementations of JSON alone. Developers not only need to familiarise themselves with each languages implementation of (say) JSON, but may need to familiarise themselves with multiple implementations even for a single language (e.g. if they move to a project that uses a different implementation). Also libraries that implement competing standards (e.g. XML, YAML etc) have their own interfaces with arbitrarily different interface decisions. This means replacing XML with JSON (or supporting both) is not supported well by the libraries as there is no common interface.

Another reason having an incomplete standard library is an issue is that often projects, especially large closed source projects, often build up their own base library of missing functionality. This functionality may be small missing helper functions (e.g. capitalise the first letter of a string), or larger libraries (such as a matrix mathematics). For closed source projects these base libraries are rarely shared outside the company, meaning there may be thousands and perhaps tens of thousands or more implementation of very basic functionality that is continually implemented and re-implemented. Stack Overflow is full of questions asking how to do simple things that should already be in standard libraries.

This problem will only get worse as more and more languages are added into circulation further fragmenting the programming into more and more ecosystems. Deep experience with C++ and the C and STL libraries does not transfer over to C#.

## Introducing Project Alexandria

Project Alexandria is a moonshot project that aims to create a universal, and extremely extensive multi-language library that can be used as a drop-in replacement for each target language's standard library along with a host of third party libraries.

### Standardised Interfaces

The first part is creating a set of standard interfaces for functionality. It's not possible for a single interface to be used verbatim across all languages as language differences require interface differences. What is appropriate for an object oriented language is different than a functional language, what is appropriate for a statically typed language may not be appropriate for a dynamically typed language.

However, the goal is to unify the interfaces as much as pragmatically possible while adhering to language idioms while removing arbitrary differences. Ideally a single (or a small number of) interfaces could be created with an expressive enough syntax that allows them to be converted to all target languages. These interfaces would be designed so that components could work together. Even if Project Alexandria only delivered standardised interfaces it would be a win for new languages as the design could be re-used reducing the cost of creating a new standard library.

A toy example of a generic interface might be the following:

    template<typename FLOATING> FLOATING cos(FLOATING x)

This interface defines that there is a cosine function that can take any floating point number (float, double, or long double or half double if supported).

For example in languages such as Python that do not let the user specify the type, it may output a single function interface:

    def cos(x)

For languages such as C that do specify types and do not have function overloading it may produce two functions with different names (using a standard template), e.g.

    double cos(double x)
    float cosf(float x)

For language such as C++ which support generics (templates) it could output:

    template<T> T cos(T x)

### Building the library

The library could be created using the following two techniques:

#### Intgegration

Project Alexandria could be built by taking existing, compatible open source implementations and modifying them to adhere to the given interface. There is no need to re-write all the functionality from scratch.

#### Transpilation

Project Alexandria would ideally need a language (new or existing) where interfaces could be written in an expressive source language and be transpiled to multiple target languages.

It may be possible to extend this so that some implementation code can also be transpiled. This need not be perfect. This does not need to work for all code. It also does not need a single source implementation that must be transpiled to all languages. Having different sources for different language groups is acceptable. The goal is simply to reduce the amount of code duplication.

If there is not an existing project that can be leveraged to do this then a new language would need to created. This language is dubbed "Alexandrina".

### Licensing

If this library is to be widely used and developed, it should use a permissive open source licence. Use of a restrictive open source licence such as the GPL would create a schism where commercial code would need to have their own implementations which would significantly reduce the utility of the library. It would also reduce the pool of labour as employees at non open source companies would not use or contribute to it.

### Versioning

One reason that standard libraries remain small is the issues of backwards compatibility and maintainability. If you add something to a standard library then you either need to support it forever or break old code in upgrades. For this reason (and others) libraries are judicious in what they add and the library itself often lives with the cost of inevitable poor design choices.

If this library is to be extensive  it cannot be overly cautious in functionality that it adds in fear of perpetual support. If this library is to be long lived, it must be able to revisit design choices that will involve non-backwards compatible decisions.

Backwards compatibility issues could be resolved in the following ways:

1. Periodically at a fixed schedule (fixed at some time period between 1-3 years) the library's major version will be increased. This change indicates that the library contains non-backwards compatible changes, for example components have been removed or their interfaces have changed. Older versions of the library will still be maintained and available. New, non-backwards compatible functionality could be ported back and wrapper functions created. Projects in maintenance mode that do not wish to incur the costs of upgrading are not forced to. It is crucial that this process happens on a frequent and regular basis allowing gentle upgrades and planning. If this happens too infrequently it will create rifts (e.g. Python 2.x, 3.x) encouraging users to dwell too long on older versions, it it happens too frequently it will make upgrading too difficult and fragment the ecosystem too much.

2. Converters could be written to fully or partially upgrade existing code bases to use newer versions of the library. These converters could take advantage in the LLVM tool chain which already brings functionality that can be used here (language parsers, code layout tools). However, given the hopefully large number of target languages this may not to be available for all languages, for all changes for all versions.

Each version would be named and versioned based on the year of compatibility. For example if incompatible versions were released every 3 years (copying the C++ language cycle, e.g. C++11, C++14, C++17 etc), you might have the following versions: Alexandria11.1, Alexandria14.2, Aelxandria17.0.0 etc. However if no backwards incompatible changes were required, then the the period could be extended, e.g.
Alexandria21.0.0 instead of Alexandria20.0.0.

### Delivery

Project Alexandria would create releasable libraries/modules in a form suitable for each target language (e.g Python packages, Ruby gems, Java maven and gradle artifacts, Perl modules, C++ vcpkg libraries, APT developer library packages etc).

### The Benefits

This library would bring a myriad of benefits:

* Creating a library could be done once and transpiled to multiple languages. The work required to create a library for multiple languages would become independent of the number of languages it supports.
* Projects using the library would reduce the number of external libraries they need to include which minimises all the problems that this currently causes.
* Having a large library allows standardisation of the components in that library.
* Having a large library allows common, higher level interfaces to develop (e.g. between JSON, XML etc parsers).
* Having the mindset of creating an extensive library can mean that simple frequently repeated tasks can be added to the library rather than being constantly re-written.
* Having a large library not only brings together low level components but can also encourage design and development of holistic higher level components that can be used in more varied ways. Higher level components will have their source building blocks readily accessible and standardised. For example if a layout engine was added to the repository which already contains HTML+CSS, LaTeX, Text and Terminal support it could provide universal layout capabilities for each.
* Switching the coding language used in a project becomes easier if the same library with similar interfaces is available in both.
* Creating new languages becomes easier as language authors can simply transpile the library to their language.
* Learning new languages becomes easier if the new language has the same library with similar interfaces.
