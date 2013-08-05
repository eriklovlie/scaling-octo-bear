scaling-octo-bear
=================

About
-----

Very simple tool to extract some basic complexity metrics for C++ source code.
Currently only extracts function lengths.

Building
--------

Follow the steps here to get the llvm and clang sources:

http://clang.llvm.org/docs/LibASTMatchersTutorial.html

Then do this:

    cd llvm/tools/clang/tools/extra
    git clone https://github.com/eriklovlie/scaling-octo-bear.git complexity-check

Then cd to your build directory and run ninja to build.
