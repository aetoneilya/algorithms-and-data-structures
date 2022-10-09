CXX=g++
CXXFLAGS=-c -pedantic -Wall -Wextra -Wshadow -Wconversion -Wunreachable-code -std=c++17

LAB1SRC=lab_1_list/main.cpp
LAB1OBJ=$(LAB1SRC:.cpp=.o)
LAB1EXECUTABLE=list

LAB2SRC=lab_2_bstree/main.cpp
LAB2OBJ=$(LAB2SRC:.cpp=.o)
LAB2EXECUTABLE=bst

.PHONY: all build test gcov_report style clean leaks rebuild

all: build

lab_1: $(LAB1OBJ)
	$(CXX) $^ -o $(LAB1EXECUTABLE) $(LDFLAGS)

lab_2: $(LAB2OBJ)
	$(CXX) $^ -o $(LAB2EXECUTABLE) $(LDFLAGS)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

style:
	cppcheck --std=c++17 --enable=all --suppressions-list=suppressions.txt .

clean:
	rm -rf $(LAB1OBJ) $(LAB2OBJ) $(LAB1EXECUTABLE) $(LAB2EXECUTABLE)

rebuild: clean all
