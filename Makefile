CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -std=c++17 -pthread
SRC = src/main.cpp
TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) include/skip_list.h
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)