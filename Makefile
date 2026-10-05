CXXFLAGS = -Wall -Wextra -std=c++17

test_testutils: test_testutils.cpp testutils.h
	g++ $(CXXFLAGS) test_testutils.cpp -o test_testutils

clean:
	rm -f test_testUtils
