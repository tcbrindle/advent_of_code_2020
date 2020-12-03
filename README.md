
# C++20 solutions to Advent of Code 2020 #

This repository contains my solutions to [Advent of Code 2020](https://adventofcode.com/2019/). The code is written in C++20.

The source file(s) for each day are in their own directories. There is no build system or anything like that: just `cd` to a directory and compile using the command line. The solutions have been tested with GCC 10 and AppleClang 12. They may or may not work with MSVC.

To save typing, the file `common.hpp` in this directory `#include`s many headers which are re-used in most of the solutions. You may wish to precompile this file to improve your build times.

Most of the solutions use an experimental work-in-progress functional programming library called libflow.