/*_testutils.h______________________________________________________ 
|  Utilities for writing C++ test programs.                         |
|                                                                   |
|  Copyright (c) 2026 Sameer Kale [skale835@proton.me]              |
|  SPDX-License-Identifier: MIT                                     |
|                                                                   |
|__________________________________________________________________*/
// =========== HEADERS =========================================== //
  #ifndef TESTUTILS_H
  #define TESTUTILS_H

  #include <iostream>
  #include <string>
  #include <stdexcept>
  #include <exception>
  namespace test {        /* Call all functions using test::fn() */


// =========== FORMATTING ======================================== //

// ----------- Colors -------------------------------------------- //
  const std::string TERM_RED      = "\033[31m";
  const std::string TERM_RED_BOLD = "\033[91m";
  const std::string TERM_YEL      = "\033[33m";
  const std::string TERM_YEL_BOLD = "\033[93m";
  const std::string TERM_GRN      = "\033[32m";
  const std::string TERM_CYN      = "\033[36m";
  const std::string TERM_MAG      = "\033[35m";
  const std::string TERM_GRA      = "\033[90m";
  const std::string TERM_UNC      = "\033[0m";
  #define TU_RED(a) TERM_RED + a + TERM_UNC
  #define TU_YEL(a) TERM_YEL + a + TERM_UNC
  #define TU_YEL_BOLD(a) TERM_YEL_BOLD + a + TERM_UNC
  #define TU_RED_BOLD(a) TERM_RED_BOLD + a + TERM_UNC
  #define TU_GRN(a) TERM_GRN + a + TERM_UNC
  #define TU_CYN(a) TERM_CYN + a + TERM_UNC
  #define TU_MAG(a) TERM_MAG + a + TERM_UNC
  #define TU_GRA(a) TERM_GRA + a + TERM_UNC

  /* In strings, use TU_CYN("some phrase") to make "some phrase" cyan. */

// =========== Prints ============================================ //
// ----------- printTitle() -------------------------------------- //
  inline void printTitle(const std::string& titleText) {
    std::cout << TU_YEL_BOLD(
      "\n============================================================\n"
      + titleText +
      "\n============================================================\n");
  }

// ----------- printHeading() ------------------------------------ //
  inline void printHeading(const std::string& headingText) {
    std::cout <<
      "\n------------------------------------------------------------\n";
    std::cout << TU_YEL(headingText) << '\n';
    std::cout <<
      "------------------------------------------------------------\n";
  }

// ----------- printNote() --------------------------------------- //
  inline void printNote(const std::string& noteText) {
    std::cout <<
       TU_YEL("NOTE:") + "\t" + noteText + "\n" << std::endl;
  }

// ----------- printResult() ------------------------------------- //
  inline void printResult(const std::string& testName, bool result) {
    std::cout << (result? TU_GRN("[PASS]")
                    : TU_RED("[FAIL]"))
         << " " << testName << std::endl;
  }

// ----------- printVariable() ----------------------------------- //
  template <typename T>
  inline void printVariable(const std::string &variableName, 
                            const T& variable) {
    std::cout << "       " + variableName + " = " << variable << std::endl;
  }

// ----------- printArray() -------------------------------------- //
  template <typename T>
  inline void printArray(const std::string& arrayName, 
                         const T& array) {
    std::cout << "       " + arrayName + " = {" ;
    for (const auto & arrayElement : array) {
      std::cout << arrayElement << " ";
    }
    std::cout << "}" << std::endl;
    return;
  }

// ----------- printOutput() ---------------------------------TEST- //
  inline void printOutput(const std::string& outName,
                          const std::string& termOutput) {
    std::string output = termOutput;
    std::size_t lineNo = 1;
    std::cout << TU_MAG("OUTPUT\t" << outName) 
                 << ":\n" + TU_GRA(std::to_string(lineNo++)) + "\t";
    std::size_t pos = 0;
    do {
      pos = output.find("\n",pos);
      if (pos++ + 1>= output.size()) break;
      output.insert(pos,TU_GRA(std::to_string(lineNo++)) + "\t");
    } while (1);
    std::cout << output;
//  std::cout << TU_CYN("\t--------") << std::endl;
    return;
  }
// =========== EXCEPTION CATCHING ================================ //
    /* EXPECT_OK will try goodBlock. If error is thrown, test
       will fail. No thrown error, pass. Hence "Expect OK".
       printResult is called either way. */
  #define EXPECT_OK(goodBlock, blockName) do {                     \
    try {                                                          \
      goodBlock;                                                   \
      test::printResult(blockName, true);                          \
    }                                                              \
    catch (const std::exception& err) {                            \
      test::printResult(blockName, false);                         \
      std::cout << "\tException: " << err.what() << std::endl;     \
    }                                                              \
    catch (...) {                                                  \
      test::printResult(blockName, false);                         \
      std::cout << "\tArbitrary exception thrown"                  \
                << std::endl;                                      \
    }                                                              \
  } while (0)

     /* EXPECT_NG will try badBlock. If the specified expErr
        (must be std::exception type)  is thrown, then
        pass. Otherwise, fail.  "Expect NG". As with EXPECT_OK, 
        printResult is called either way.*/
 #define EXPECT_NG(badBlock, blockName, expErr) do {               \
    try {                                                          \
      badBlock;                                                    \
      test::printResult(blockName, false);                         \
    }                                                              \
    catch (const expErr &err) {                                    \
      test::printResult(blockName, true);                          \
      std::cout << "\tException: " << err.what() << std::endl;     \
    }                                                              \
    catch (const std::exception& err) {                            \
      test::printResult(blockName, false);                         \
      std::cout << "\tException: " << err.what() << std::endl;     \
    }                                                              \
    catch (...) {                                                  \
      test::printResult(blockName, false);                         \
      std::cout << "\tArbitrary exception thrown"                  \
                << std::endl;                                      \
    }                                                              \
  } while (0)


// =========== CLI COMMAND EVALUATION ============================ //
  int runCommand(std::string sysCmd,
                 std::string *stdoutTo,
                 std::string* stderrTo,
                 int* returnTo) {
    const std::size_t bufsize = 128;
    FILE *pipe = popen(sysCmd.c_str(), "r");
    char buf[bufsize];
    char* fgets_ptr = buf;
    while(1) {
      fgets_ptr = fgets(buf, bufsize, pipe);
      if (fgets_ptr == NULL) break;
      (*stdoutTo).append(buf);
    } 
    fgets(buf, bufsize, pipe);
    (*stdoutTo).append(buf);
    *stderrTo = "";
    *returnTo = pclose(pipe);
    return 0;
  }
 
  } // namespace test
  #endif //TESTUTILS_H

//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ //
