/*_testutils.h______________________________________________________ 
|  Utilities for writing test programs.                             |
|                                                                   |
|  Copyright (c) 2026 Sameer Kale [skale835@proton.me]              |
|  SPDX-License-Identifier: MIT                                     |
|                                                                   |
|__________________________________________________________________*/
// =========== HEADERS =========================================== //
  #ifndef TESTUTILS_H
  #define TESTUTILS_H
  #ifdef __cplusplus
    #define TU_ISCPP 1
    #define TU_ISC   0
  #else
    #define TU_ISC   1
    #define TU_ISCPP 0
  #endif //__cplusplus

  #if TU_ISC             /* Some structures for a future functionality
  #include <stdio.h>        for C were written, though it does *not*
  #include <string.h>       actually have any C functionality yet. */
  #endif// TU_ISC

  #if TU_ISCPP
  #include <iostream>
  #include <string>
  #include <stdexcept>
  #include <exception>
  namespace test {        /* Call all functions using test::fn() */
  #endif // TU_ISCPP


// =========== FORMATTING ======================================== //

// ----------- Colors -------------------------------------------- //
  #if TU_ISCPP
  const std::string TERM_RED      = "\033[31m";
  const std::string TERM_YEL      = "\033[33m";
  const std::string TERM_YEL_BOLD = "\033[93m";
  const std::string TERM_GRN      = "\033[32m";
  const std::string TERM_CYN      = "\033[36m";
  const std::string TERM_UNC      = "\033[0m";
  #define TU_RED(a) TERM_RED + a + TERM_UNC
  #define TU_YEL(a) TERM_YEL + a + TERM_UNC
  #define TU_YEL_BOLD(a) TERM_YEL_BOLD + a + TERM_UNC
  #define TU_GRN(a) TERM_GRN + a + TERM_UNC
  #define TU_CYN(a) TERM_CYN + a + TERM_UNC
  #endif //TU_ISCPP

  #if TU_ISC
  #endif //TU_ISC

  /* In strings, use TU_CYN("some phrase") to make "some phrase" cyan. */

// =========== Prints ============================================ //
// ----------- printTitle() -------------------------------------- //
  #if TU_ISCPP
  inline void printTitle(const std::string& titleText) {
    std::cout << TU_YEL_BOLD(
      "\n============================================================\n"
      + titleText +
      "\n============================================================\n");
  }
  #endif //TU_ISCPP

// ----------- printHeading() ------------------------------------ //
  #if TU_ISCPP
  inline void printHeading(const std::string& headingText) {
    std::cout <<
      "\n------------------------------------------------------------\n";
    std::cout << TU_YEL(headingText) << '\n';
    std::cout <<
      "------------------------------------------------------------\n";
  }
  #endif //TU_ISCPP

// ----------- printResult() ------------------------------------- //
  #if TU_ISCPP
  inline void printResult(const std::string& testName, bool result) {
    std::cout << (result? TU_GRN("[PASS]")
                    : TU_RED("[FAIL]"))
         << " " << testName << std::endl;
  }
  #endif //TU_ISCPP

// ----------- printVariable() ----------------------------------- //
  #if TU_ISCPP
  template <typename T>
  inline void printVariable(std::string variableName, T variable) {
    std::cout << "       " + arrayName + " = " + variable << std::endl;
  }
  #endif //TU_ISCPP

// ----------- printArray() -------------------------------------- //
  #if TU_ISCPP
  template <typename T>
  inline void printArray(std::string arrayName, T array) {
    std::cout << "       " + arrayName + " = {" ;
    for (const auto & arrayElement : array) {
      std::cout << arrayElement << " ";
    }
    std::cout << "}" << std::endl;
  }
  #endif //TU_ISCPP


// =========== Tests ============================================= //
    /* EXPECT_OK will try goodBlock. If error is thrown, test
       will fail. No thrown error, pass. Hence "Expects OK".
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

     /* EXPECT_NG will try badBlock. If the expErr error is thrown, 
       pass. Otherwise, fail.  "Expects NG". As with EXPECT_OK, 
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



 
  #if TU_ISCPP
  } // namespace test
  #endif //TU_ISCPP
  #endif //TESTUTILS_H

//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ // TEST
