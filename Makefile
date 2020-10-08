SRC := ./src
SRCS := $(wildcard $(SRC)/*.cpp)
HEADERS := $(wildcard $(SRC)/*.h)
CXX = clang++
CXXFLAGS := -O3 -Wall -Wextra

all: chess

chess: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

.PHONY: clean
clean:
	$(RM) chess *.d
