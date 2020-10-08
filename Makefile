SRC := ./src
SRCS := $(wildcard $(SRC)/*.cpp)
HEADERS := $(wildcard $(SRC)/*.h)
CXX = clang++
CXXFLAGS := -03 -Wall -Wextra -MMD -fopenmp 

all: chess

chess: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

.PHONY: clean
clean:
	$(RM) chess *.d
