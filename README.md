scaling-octo-bear
=================

About
-----

Very simple tool to extract some basic complexity metrics for C++ source code.
Currently only extracts function lengths.

Do we need another tool for style checking?
-------------------------------------------

This depends on what is meant by "style". There are many tools which 
focus on the superficial stuff (placement of curly braces, the kind of issues 
most coding standards focus on instead of the important stuff) and static 
analysis (correctness).

I haven't been able to find a good tool which

* checks the important stuff related to coding style (i.e. readability, maintainability, complexity)
* doesn't force you to follow a particular style (as in the formatting of the source code text)
* is robust and works with recent C++ standards (i.e. not just a bunch of regular expressions)

These are some other tools out there:

* pmccabe: haven't tested it, but seems good. Currently has a warning about not supporting templates.
* uncrustify: nice source code formatter.
* astyle: source code formatter. A bit buggy and doesn't work with recent C++ standards.
* Google's cpplint. Requires you to follow the Google C++ style.
* Static analysis tools, e.g. Cppcheck and Clang[1] tools. They focus on correctness, not readability.
* Various other linting tools. Those which even support C++ seem less than robust.

[1] Why haven't the Clang people already made such a tool? The interwebs are full of formatters and
translaters, but very few good tools for extracting basic software metrics. Oh well, at least they
made a nice library.

Requirements (i.e. what should the tool do):
--------------------------------------------

* extract important quality metrics (function length, nesting depth, cyclomatic complexity, etc)
* output results in machine readable format (e.g. JSON)
* robust support for C++ (including recent standards, e.g. C++11)
* make no assumption on the formatting of the source code, and no attempt at reformatting.
* easily integrated in existing build flows (Makefiles, cmake, scripts, Jenkins, etc)

Building
--------

Follow the steps here to get the llvm and clang sources:

http://clang.llvm.org/docs/LibASTMatchersTutorial.html

Then do this:

    cd llvm/tools/clang/tools/extra
    git clone https://github.com/eriklovlie/scaling-octo-bear.git complexity-check
    echo "add_subdirectory(complexity-check)" >> CMakeLists.txt

Then cd to your build directory and run ninja to build.
