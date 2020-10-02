SRC := ./src
SRCS := $(wildcard $(SRC)/*.cpp)
CXX = clang++
CXXFLAGS := -O3 -Wall -Wextra -MMD -fopenmp

all: chess

chess: $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

.PHONY: clean
clean:
	$(RM) chess *.d
