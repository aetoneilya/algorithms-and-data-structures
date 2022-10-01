#include "bstree.h"

int main() {
  Bst<int, char> b;
  b.insert(1, 'c');
  b.insert(2, 'b');
  b.insert(3, 'd');
  b.insert(0, 'a');
  b.insert(-1, 'g');
  std::cout << b.height();
}