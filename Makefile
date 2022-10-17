CXX=g++
CXXFLAGS=-c -pedantic -Wall -Wextra -Wshadow -Wconversion -Wunreachable-code -std=c++17

LAB1SRC=lab_1_list/main.cpp
LAB1OBJ=$(LAB1SRC:.cpp=.o)
LAB1EXECUTABLE=list

LAB2SRC=lab_2_bstree/main.cpp
LAB2OBJ=$(LAB2SRC:.cpp=.o)
LAB2EXECUTABLE=bst

LAB3SRC=lab_3_avl_tree/main.cpp
LAB3OBJ=$(LAB3SRC:.cpp=.o)
LAB3EXECUTABLE=avl

.PHONY: all build test gcov_report style clean leaks rebuild

all: build

lab_1: $(LAB1OBJ)
	$(CXX) $^ -o $(LAB1EXECUTABLE) $(LDFLAGS)

lab_2: $(LAB2OBJ)
	$(CXX) $^ -o $(LAB2EXECUTABLE) $(LDFLAGS)

lab_3: $(LAB3OBJ)
	$(CXX) $^ -o $(LAB3EXECUTABLE) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

style:
	cppcheck --std=c++17 --enable=all --suppressions-list=suppressions.txt .

clean:
	rm -rf $(LAB1OBJ) $(LAB2OBJ)  $(LAB3OBJ) $(LAB1EXECUTABLE) $(LAB2EXECUTABLE) $(LAB3EXECUTABLE)

rebuild: clean all
