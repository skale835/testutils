/*_test_testUtils.cpp________________________________________________
|  Test program for testUtils.h.                                     |
|                                                                    |
|  Copyright (c) 2026 Sameer Kale [skale835@proton.me]               |
|  SPDX-License-Identifier: MIT                                      |
|                                                                    |
|___________________________________________________________________*/
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


// ----------- printVariable() ------------------------------------ //
    test::printHeading("printVariable()");
    test::printVariable("name","value");
    test::printVariable("name",56.3);


// ----------- printArray() --------------------------------------- //
    test::printHeading("printArray()");

    std::vector<int> intArray {1, 2, 3, 4};
    std::vector<std::string> stringArray {"one", "two", "three"};

    test::printArray("intArray", intArray);
    test::printArray("stringArray", stringArray);

// ----------- printNote() ---------------------------------------- //
    test::printHeading("printNote()"); 
    test::printNote("This is a note");

// ----------- EXPECT_OK ------------------------------------------ //
    test::printHeading("EXPECT_OK");

    test::printNote("Result should be [PASS] [FAIL] [FAIL]");

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
    test::printNote("Result should be [PASS] [FAIL] [FAIL] [FAIL]");

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

// ----------- runCommand() --------------------------------------- //
    {
    test::printHeading("runCommand()");
    std::string so;
    std::string eo;
    int cr;
    test::printResult("runCommand()$: ls -alh :$ No Error",
                      !test::runCommand("ls -alh",&so, &eo, &cr));
    test::printOutput("stdout",so);
    test::printOutput("stderr",eo);
    test::printResult("command return status OK",!cr);
    }

// --------- Return ---------------------------------------------- //
    test::printHeading("\tTEST COMPLETE");
    return 0;
  }

//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ //
