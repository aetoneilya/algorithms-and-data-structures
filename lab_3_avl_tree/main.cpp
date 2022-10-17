#include "avl_bstree.h"

int main() {
  AvlBst<int, char> b;
  b.insert(1, 'c');
  b.insert(2, 'b');
  b.insert(3, 'd');
  b.insert(0, 'a');
  b.insert(-1, 'g');

  for (auto it = b.rbegin(); it != b.rend(); it++) std::cout << *it;
  std::cout << std::endl;
  b.remove(1);
  for (auto it = b.rbegin(); it != b.rend(); it++) std::cout << *it;
}