/*_test_testUtils.cpp________________________________________________
|  Test program for testUtils.h.                                    |
|                                                                    |
|  Copyright (c) 2026 Sameer Kale [skale835@proton.me]               |
|  SPDX-License-Identifier: MIT                                      |
|                                                                    |
|__________________________________________________________________*/
// =========== HEADERS ============================================ //
  #include "testutils.h"

  #include <stdexcept>
  #include <string>
  #include <vector>


// =========== MAIN ============================================== //
  int main() {
    test::printTitle("testUtils Test");


// ----------- printResult() -------------------------------------- //
    test::printHeading("printResult()");

    test::printResult("true condition",  2 + 2 == 4);
    test::printResult("false condition", 2 + 2 == 5);


// ----------- printArray() --------------------------------------- //
    test::printHeading("printArray()");

    std::vector<int> intArray {1, 2, 3, 4};
    std::vector<std::string> stringArray {"one", "two", "three"};

    test::printArray("intArray", intArray);
    test::printArray("stringArray", stringArray);


// ----------- EXPECT_OK ------------------------------------------ //
    test::printHeading("EXPECT_OK");

    EXPECT_OK(
      {
        int a = 2;
        int b = 3;
        int c = a + b;
        (void)c;
      },
      "normal block completes"
    );

    EXPECT_OK(
      throw std::runtime_error("intentional std::exception"),
      "std::exception causes EXPECT_OK failure"
    );

    EXPECT_OK(
      throw 42,
      "arbitrary exception causes EXPECT_OK failure"
    );


// ----------- EXPECT_NG ------------------------------------------ //
    test::printHeading("EXPECT_NG");

    EXPECT_NG(
      throw std::out_of_range("intentional expected exception"),
      "expected exception is thrown",
      std::out_of_range
    );

    EXPECT_NG(
      {
        int a = 1;
        (void)a;
      },
      "no exception causes EXPECT_NG failure",
      std::out_of_range
    );

    EXPECT_NG(
      throw std::runtime_error("intentional wrong exception"),
      "wrong std::exception causes EXPECT_NG failure",
      std::out_of_range
    );

    EXPECT_NG(
      throw 42,
      "arbitrary exception causes EXPECT_NG failure",
      std::out_of_range
    );

    return 0;
  }

//+++++++++++ EOF +++++++++++++++++++++++++++++++++++++++++++++++++ //
