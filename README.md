# testutils

A small C++ header that provides utilities for writing test programs for your functions. It is deliberately small enough so that someone can spend a few minutes reading the source, and then just use it. ChatGPT can *very* effectively use testutils.h to generate a test script, and that test script will be very readable as well. Deliberately avoided are complex `TEST_ASSERT_ABC_TO_JMP` macros and `function_handle_wrap_catch_error` objects. Spend more time on your actual code.

## Installation

Copy `testutils.h` into your project directory and include it in your test program:
```cpp
#include "testutils.h"
```
No separate source file or library is required.

## Use
testutils consists of several functions that provide building blocks for you to construct your
test program. There is no initialization or anything like that, just use it.

## Test function

`test_testutils.cpp` provides a test program for the class. It uses testutils to test testutils! 

The test program is provided to assist you if you modify testutils for your needs.

## License

testutils is released under the MIT License. See `LICENSE` for the full license text.

Sharing improvements is not compulsory, but is still welcome. See my email below.

Copyright (c) 2026 Sameer Kale [skale835@proton.me].
