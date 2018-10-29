#include "Map.hpp"

#include <iostream>

using namespace std;
using namespace cs540;

int main() {
  RBT<int, int> tree;
  tree.insert({7, 6});
  tree.insert({6, 3});
  tree.insert({5, 32});
  tree.insert({4, 66});
  tree.insert({3, 993});
  tree.insert({2, 1200});
  tree.insert({1, 16});
  tree.insert({0, 15});

  cout << "Inoder Traversal of Created Tree\n";
  tree.inorder();
  cout << endl;

}
