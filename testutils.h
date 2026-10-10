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

  #include <unistd.h>
    /* pipes & file descriptors */
  #include <fcntl.h>
    /* Definition of O_* constants */
  #include <sys/wait.h>
    /* waitpid() */
  #include <poll.h>
    /* poll() */
  #include <stdlib.h>
    /* exit() */
  #include <string.h>
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
    std::cout << "\t" + variableName + " = " << variable << std::endl;
  }

// ----------- printArray() -------------------------------------- //
  template <typename T>
  inline void printArray(const std::string& arrayName, 
                         const T& array) {
    std::cout << "\t" + arrayName + " = { " ;
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
    std::cout << TU_MAG("OUTPUT\t" << outName);

    if (termOutput.length() == 0) {
      std::cout << TU_GRA("\nNONE") << std::endl;
      return;
    }
    else {
      /* If last character is NOT `\n` add one. */
      if (output.back() != '\n') output.append("\n");
    }

    std::cout << ": "<< output.size() << "\n" 
              << TU_GRA(std::to_string(lineNo++)) + "\t";
    std::size_t pos = 0;
    #define MAX_LINES 16
    do {
      pos = output.find("\n",pos);
      if (pos++ + 1>= output.size()) break;
      output.insert(pos,TU_GRA(std::to_string(lineNo++)) + "\t");
    } while (1);//lineNo <= MAX_LINES);
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
    *stderrTo = "";
    *returnTo = pclose(pipe);
    return 0;
  }
 
  int runCommandFork(std::string cmd,
                     std::string *stdoutTo,
                     std::string *stderrTo,
                     int         *returnTo) {

    //-> Create pipes for stdout and stderr
    int outP[2];
    int errP[2];
    std::cout << "Milestone 1" << std::endl;
    if (pipe(outP)) throw std::runtime_error("out pipe failed");
    std::cout << "Milestone 2" << std::endl;
    if (pipe(errP)) throw std::runtime_error("err pipe failed");
    std::cout << "Milestone 3" << std::endl;

    //-> Fork
    pid_t pid = fork();
    if (pid == -1) throw std::runtime_error("fork failed");

    //-? Child 
      /* Child process connects stdout, stderr to pipes, then 
         exec(cmd). */
    if (!pid) {
      if (close(outP[0])) throw std::runtime_error("close out");
      if (close(errP[0])) throw std::runtime_error("close err");
      std::cout << "Child first" << std::endl;
      if (dup2(outP[1],1)==-1) 
        throw std::runtime_error("out dup failed");
      if (dup2(errP[1],2)==-1) 
        throw std::runtime_error("err dup failed");
      execlp("/bin/sh", "sh", "-c", cmd.c_str(), (char*) NULL);
    }

    //-> Parent awaits child's termination, gets return val.
    if (close(outP[1])) throw std::runtime_error("close out");
    if (close(errP[1])) throw std::runtime_error("close err");

    //-> Read stdout and stderr
    #define BUF_SIZE 128
    char buf[BUF_SIZE];
    int nBytesReadOut = 0;
    int nBytesReadErr = 0;
    do {
      nBytesReadOut = read(outP[0], buf, BUF_SIZE);
      if (nBytesReadOut < 0) throw std::runtime_error("Read failed.");
      if (nBytesReadOut > 0) {(*stdoutTo).append(buf,0,nBytesReadOut);
                           memset(buf,0,BUF_SIZE);}

      nBytesReadErr = read(errP[0], buf, BUF_SIZE);
      if (nBytesReadErr < 0) throw std::runtime_error("Read failed.");
      if (nBytesReadErr > 0) {(*stderrTo).append(buf,0,nBytesReadErr);
                           memset(buf,0,BUF_SIZE);};

    } while (nBytesReadOut + nBytesReadErr);

    int stat_val;
    if (waitpid(pid, &stat_val, 0)==-1)
      throw std::runtime_error("waitpid failed");
    std::cout << "Child stat_val: " << stat_val << std::endl;
    if (WIFEXITED(stat_val))  
      *returnTo = WEXITSTATUS(stat_val); 
    std::cout << "Child process PID: " << pid << std::endl;

    
    std::cout << "stderr done\n";
    
    return 0; 
  }

  int runCommandPoll(std::string cmd,
                     std::string *stdoutTo,
                     std::string *stderrTo,
                     int *returnTo) {
  
    //-> Create pipes for stdout and stderr
    int outP[2] = {0};
    int errP[2] = {0};
    if (pipe(outP)) throw std::runtime_error("out pipe failed");
    if (pipe(errP)) throw std::runtime_error("err pipe failed");
    printArray("outP",outP);
    printArray("errP",errP);
    //-> Fork
    pid_t pid = fork();
    if (pid == -1) throw std::runtime_error("fork failed");

    //-? Child 
      /* Child process connects stdout, stderr to pipes, then 
         exec(cmd). */
    if (!pid) {
      //-> dup() the pipe ends to stdout and stderr
      if (close(outP[0])) throw std::runtime_error("C close out");
      if (close(errP[0])) throw std::runtime_error("C close err");
      std::cout << "Child first" << std::endl;
      if (dup2(outP[1],1)==-1) 
        throw std::runtime_error("out dup failed");
      if (dup2(errP[1],2)==-1) 
        throw std::runtime_error("err dup failed");
      if (close(outP[1])) throw std::runtime_error("C close out");
      if (close(errP[1])) throw std::runtime_error("C close err");
      
      //-> exec(cmd)
      execlp("/bin/sh", "sh", "-c", cmd.c_str(), (char*) NULL);
      /* Note that if exec() is successful, this process will 
         terminate, effectively ending here. Subsequent lines 
         will only execute if exec() fails. */
      _exit(EXIT_FAILURE);
       
    }
    std::cout << "Parent is running" << std::endl;
    //-> Parent: Close leftover pipe ends.
    if (close(outP[1])) throw std::runtime_error("P close out");
    if (close(errP[1])) throw std::runtime_error("P close err");

    //-O Collect output from outP and/or errP. Move on when done.
    #define BUF_SIZE 128
    #define MAX_BLOCK_TIME 1000 // ms until reloop
    #define MAX_RELOOPS 3 // maximum reloops until giveup
    char buf[BUF_SIZE];
    int nBytesReadOut = 0;
    int nBytesReadErr = 0;
      /* Array containing outP, errP pollfd structs */
    struct pollfd pArray[2];
    pArray[0].fd = outP[0];
    pArray[0].events = POLLIN;
    pArray[1].fd = errP[0];
    pArray[1].events = POLLIN;
    int pollResult = 0;
    int nReloops = 0;
    int stopLooping = 0;

    do {
      pollResult = poll(pArray,2,MAX_BLOCK_TIME);
        /* Lots of error checking; the "bad" paths are marked 
           with //! and the "good" paths with //~ */

        /* Receive from outP */
      if (pollResult > 0) { //~
        nReoops = 0;
        if (pArray[0].revents & POLLIN) { //~
          nBytesReadOut = read(outP[0], buf, BUF_SIZE);
          if (nBytesReadOut < 0) { //!
            throw std::runtime_error("Read outP failed.");
          }
          if (nBytesReadOut > 0) { //~
            (*stdoutTo).append(buf,0,nBytesReadOut);
            memset(buf,0,BUF_SIZE);
          }
        }
        else if (pArray[0].revents & POLLNVAL) { //!
          throw std::runtime_error("outP[0] invalid");
        }
        else if (pArray[0].revents & POLLERR) {//!
          throw std::runtime_error("outP[0] error"); 
        }

        /* Receive from errP */
        if (pArray[1].revents & POLLIN) { //~
          nBytesReadErr = read(errP[0], buf, BUF_SIZE);
          if (nBytesReadErr < 0) {//!
            throw std::runtime_error("Read errP failed.");
          }
          if (nBytesReadErr > 0) { //~
            (*stderrTo).append(buf,0,nBytesReadErr);
            memset(buf,0,BUF_SIZE);
          }
        }
        else if (pArray[1].revents & POLLNVAL) { //!
          throw std::runtime_error("errP[0] invalid");
        }
        else if (pArray[1].revents & POLLERR) {//!
          throw std::runtime_error("errP[0] error"); 
        }


          /* All conditions must be met to stop looping */
        stopLooping  = (nBytesReadOut < BUF_SIZE);
        stopLooping *= (nBytesReadErr < BUF_SIZE);
        stopLooping *= (pArray[0].revents & POLLHUP);
        stopLooping *= (pArray[1].revents & POLLHUP);
        stopLooping *= (nReloops >= MAX_RELOOPS);  
      }
      else if (pollResult < 0) 
        throw std::runtime_error("Poll failed");
      else nReloops++; // pollResult == 0
    } while (!stopLooping);
    
    //-> Collect return status from exec'd function
    
    std::cout << "Parent is waiting" << std::endl;
    int stat_val;
    if (waitpid(pid, &stat_val, 0)==-1)
      throw std::runtime_error("waitpid failed");
    if (WIFEXITED(stat_val))  
      *returnTo = WEXITSTATUS(stat_val); 

    return 0; 
  }

  } // namespace test
  #endif //TESTUTILS_H

//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ //
