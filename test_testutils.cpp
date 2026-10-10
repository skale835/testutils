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
    {
      test::printHeading("printResult()");

      test::printResult("true condition",  2 + 2 == 4);
      test::printResult("false condition", 2 + 2 == 5);
    }


// ----------- printVariable() ------------------------------------ //
    {
      test::printHeading("printVariable()");
      test::printVariable("name","value");
      test::printVariable("name",56.3);
    }


// ----------- printArray() --------------------------------------- //
    {
      test::printHeading("printArray()");

      std::vector<int> intArray {1, 2, 3, 4};
      std::vector<std::string> stringArray {"one", "two", "three"};

      test::printArray("intArray", intArray);
      test::printArray("stringArray", stringArray);
    }

// ----------- printNote() ---------------------------------------- //
    {
      test::printHeading("printNote()"); 
      test::printNote("This is a note");
    }

// ----------- EXPECT_OK ------------------------------------------ //
    {
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
    }


// ----------- EXPECT_NG ------------------------------------------ //
    {
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
    }

// ----------- runCommand() --------------------------------------- //
    {
      test::printHeading("RUN COMMAND");

      std::string cmd;
      std::string cmdOut;
      std::string cmdErr;
      int cmdRet = -1;

      //-> stdout only
      cmd = "printf 'stdout test'";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): stdout only");

      test::printOutput("stdout",cmdOut);
      test::printOutput("stderr",cmdErr);
      test::printVariable("return",cmdRet);

      test::printResult("stdout captured correctly",
                        cmdOut == "stdout test");
      test::printResult("stderr empty",
                        cmdErr.empty());


      //-> stderr only
      cmd = "printf 'stderr test' >&2";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): stderr only");

      test::printOutput("stdout",cmdOut);
      test::printOutput("stderr",cmdErr);
      test::printVariable("return",cmdRet);

      test::printResult("stdout empty",
                        cmdOut.empty());
      test::printResult("stderr captured correctly",
                        cmdErr == "stderr test");


      //-> stdout and stderr
      cmd = "printf 'stdout test'; printf 'stderr test' >&2";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): stdout and stderr");

      test::printOutput("stdout",cmdOut);
      test::printOutput("stderr",cmdErr);
      test::printVariable("return",cmdRet);

      test::printResult("stdout captured with mixed output",
                        cmdOut == "stdout test");
      test::printResult("stderr captured with mixed output",
                        cmdErr == "stderr test");


      //-> Exactly BUF_SIZE bytes
      cmd = "printf '%0128d' 0";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): exactly BUF_SIZE stdout");

      test::printVariable("stdout.size()",cmdOut.size());
      test::printResult("exactly BUF_SIZE bytes captured",
                        cmdOut.size() == 128);


      //-> More than BUF_SIZE bytes
      cmd = "printf '%0257d' 0";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): multiple reads");

      test::printVariable("stdout.size()",cmdOut.size());
      test::printResult("257 bytes captured",
                        cmdOut.size() == 257);


      //-> Exactly BUF_SIZE bytes on both pipes
      cmd = "printf '%0128d' 0; printf '%0128d' 0 >&2";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): BUF_SIZE stdout and stderr");

      test::printResult("128 stdout bytes captured",
                        cmdOut.size() == 128);
      test::printResult("128 stderr bytes captured",
                        cmdErr.size() == 128);


      //-> Command return status
      cmd = "exit 42";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): command exit status");

      test::printVariable("return",cmdRet);
      test::printResult("returnTo captures exit status 42",
                        cmdRet == 42);


      //-> Successful command returns zero
      cmd = "true";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): successful command");

      test::printResult("successful command returns zero",
                        cmdRet == 0);


      //-> Shell cannot find command
      cmd = "__testutils_command_does_not_exist__";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): nonexistent command");

      test::printOutput("stderr",cmdErr);
      test::printVariable("return",cmdRet);
      test::printResult("shell returns 127 for command not found",
                        cmdRet == 127);


      //-> stdout and stderr substantially larger than pipe buffer
      cmd = "i=0; while [ $i -lt 1000 ]; do "
            "printf 'OUT%04d\\n' \"$i\"; "
            "printf 'ERR%04d\\n' \"$i\" >&2; "
            "i=$((i+1)); "
            "done";
      cmdOut.clear();
      cmdErr.clear();
      cmdRet = -1;

      EXPECT_OK(test::runCommand(cmd,
                                 &cmdOut, &cmdErr, &cmdRet),
                "runCommand(): sustained stdout and stderr");

      test::printResult("large stdout captured",
                        cmdOut.size() == 8000);
      test::printResult("large stderr captured",
                        cmdErr.size() == 8000);
      test::printResult("large mixed command returns zero",
                        cmdRet == 0);
    }

// --------- Return ---------------------------------------------- //
    test::printHeading("\tTEST COMPLETE");
    return 0;
  }

//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ //
