# Project Alexandria

Project Alexandria is a universal library that transpiles to multiple languages. This allows a library to be written in a single langauge and made available to multiple target languages through the process of transpilation which takes source code in one language and converts it to multiple target languages. This project solves the current issue where frequently third party libraries are written multiple times for multiple languages. The website json.org lists over 150 implementations of JSON alone.

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
