# The Problem

Every new year brings another programming language each with its own standard library and its own library ecosystem. The popularity of each language waxes and wanes but over time the number of live languages grows. For programmers this means (re-)learning how strings are manipulated, how arrays and maps are built and traversed, how dates and times are handled, and the myriad of other operations. For language authors whose desire is to create a language, creating a standard library is a monumental distraction.

As a result most languages lack extensive libraries. The C language has a bare bones library without even a safe string abstraction; C++ is better still - and very slowly getting better - but lacks basic computer science abstractions such as trees, graphs and rarer constructs like bloom filters; Java has an extensive library but even that has holes that are plugged by third party toolkit libraries such as Apache Commons and Guava. Languages are hard enough to design and maintain without worrying about core library support.

Third party libraries help but these need to be written for each language and are there often multiple implementations for each because none can claim to be canon. There are over 150 JSON library implementations listed on json.org, with over 25 for Java alone, and this doesn't include custom implementations in project code bases.

One issue with having multiple implementations for low level common functionality (e.g. JSON parsing) is that it's not uncommon for large projects to pull in multiple implementations of the same functionality, often forcing large projects to work with multiple implementations (and sometimes multiple versions of) and multiple interfaces for the same thing.

Another issue is that often it's painful finding and pulling in third party libraries into your build tool chain. You need to research and choose the "right" third party library, you need to update your build tool chain to pull in the library, you need to update all of those libraries periodically, and as the number of dependent libraries grow the number of dependencies between those libraries scales quadratically causing their own problems. Another issue is that higher level libraries often have their own approaches for identifier naming, coding style, documentation, error handling, translation, logging and memory management that may not be compatible with your project.

# Project Alexandria

A solution to this is project Alexandria. This is an extensive library containing both low level primitives and new, higher level components that can be transpiled to as many languages idiomatically as possible.

This project will be written in an existing, but potentially annotated programming language. The use of an existing language is to help simply the project, make it easier to re-use existing code for the library, and re-use existing tools. We could leverage the LLVM tool chain to create a tool that parses annotated C++ library code that transpiles the code and creates releasable libraries/modules in each target languages (e.g Python packages, Ruby gems, Java mavern and gradle artifacts, Perl modules, C/C++ libraries etc).

For example a function to calculate the sine of a number ready for transpiling may look like:

    template <typename FLOAT> FLOAT sin(FLOAT argument)
    {
     // Implementation...
    }

In this example the typename identifier "FLOAT" means nothing to C++ but could tell the transpiler that it should generate two functions for C: a float version, and a double version.
Building & Maintaining the Library

If this library is to be widely used and developed, it should use a permissive open source licence.  Use of a restrictive open source licence such as the GPL would create a fork where commercial code would need to have their own implementations which would significantly reduce the utility of the library.

The library could be written by finding existing open source libraries with compatible licences that can be folded into the project.  This would significantly save development time and significantly increase the utility of all those libraries.

One reason that libraries remain small is the issues of backwards compatibility and maintainability. If you add something to a core library then you either need to support it forever or break old code in upgrades. For this reason (and others) libraries are judicious in what they add and the library itself often lives with the cost of inevitable poor design choices.

If this library is to be extensive  it cannot be overly cautious in functionality that it adds in fear of perpetual support. If this library is to be long lived, it must be able to revisit design choices that will involve non-backwards compatible decisions.

Backwards compatibility issues could be resolved in the following ways:

1. Periodically (every 3-5+ years) the library's major version will be increased. This change indicates that the library contains non-backwards compatible changes, for example components have been removed or their interfaces have changed. Older versions of the library will still be maintained and available. New, non-backwards compatible functionality could be ported back. Projects in maintenance mode that do not wish to incur the costs of upgrading are not forced to.

2. Converters could be written to fully or partially upgrade existing code bases to use newer versions of the library.  These converters could take advantage in the LLVM tool chain which already brings functionality that can be used here (language parsers, code layout tools). However, given the hopefully large number of target languages this may not to be available for all languages, for all changes for all versions.

# The Benefits

This library would bring a myriad of benefits:

* Creating a library could be done once and transpiled to multiple languages. The work required to create a library for multiple languages would become independent of the number of languages it supports.
* Projects using the library would reduce the number of external libraries they need to include which minimises all the problems that this currently causes.
* Having a large library allows standardisation of the components in that library.
* Having a large library allows common, higher level interfaces to develop (e.g. between JSON, XML etc parsers).
* Having the mindset of creating an extensive library can mean that simple frequently repeated tasks can be added to the library rather than being constantly re-written.
* Having a large library not only brings together low level components but can also encourage design and development of higher level components that can be used in more varied ways. Higher level components will have their source building blocks readily accessible and standardised. For example if a layout engine was added to the repository which already contains HTML+CSS, LaTeX, Text and Terminal support it could provide universal layout capabilities for each.
* Switching the coding language used in a project becomes easier if the same library with similar interfaces is available in both.
* Creating new languages becomes easier as language authors can simply transpile the library to their language.
* Learning new languages becomes easier if the new language has the same library with similar interfaces.
