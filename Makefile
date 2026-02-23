# Mesh Graph Analyzer - build, test and clean targets
# Requires a C++11 (or later) compiler: g++ / clang++ (make only).

CXX      ?= g++
CXXFLAGS ?= -std=c++11 -Wall -O2

BIN  = programa
SRCS = main.cpp menuComp0.cpp menuComp1.cpp menuComp2.cpp menuComp3.cpp \
       objeto.cpp cara.cpp vertice.cpp KDTree.cpp NodoKDTree.cpp Grafo.cpp
OBJS = $(SRCS:.cpp=.o)

TEST_BIN  = tests/test_kdtree
TEST_SRCS = tests/test_kdtree.cpp KDTree.cpp NodoKDTree.cpp vertice.cpp \
            cara.cpp objeto.cpp Grafo.cpp

.PHONY: all test clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_BIN)
	@echo "== Running test harness =="
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SRCS)

clean:
	rm -f $(OBJS) $(BIN) tests/test_kdtree tests/test_kdtree.o