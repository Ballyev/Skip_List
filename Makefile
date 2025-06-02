CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TEST_SRC = test/skip_list_test.cpp
TEST_EXE = test/skip_list_test

.PHONY: all test clean

all: test

test: $(TEST_EXE)
	@echo "Running tests..."
	@./$(TEST_EXE) || (echo "Tests failed"; exit 1)

$(TEST_EXE): $(TEST_SRC)
	@mkdir -p bin
	@if [ ! -f $(TEST_SRC) ]; then \
		echo "Error: Test file $(TEST_SRC) not found"; \
		exit 1; \
	fi
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_EXE)

clean:
	rm -rf bin