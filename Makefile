CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -std=c++17 -pthread
TEST_SRC = test/skip_list_test.cpp
TEST_EXE = test_skip_list

GTEST_LIBS = -lgtest -lgtest_main -lpthread

.PHONY: all test clean

all: test

test: $(TEST_EXE)
	./$(TEST_EXE)

$(TEST_EXE): $(TEST_SRC) include/skip_list.h
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_EXE) $(GTEST_LIBS)

clean:
	rm -f $(TEST_EXE)