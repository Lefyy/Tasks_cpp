#include <iostream>
#include <string>
#include <vector>

#include "binary_tree.h"

template <typename T>
void printVector(const std::vector<T>& values) {
    for (const auto& value : values) {
        std::cout << value << " ";
    }
    std::cout << "\n";
}

int main() {
    BinaryTree<int> intTree;
    intTree.insert(5);
    intTree.insert(3);
    intTree.insert(7);
    intTree.insert(1);
    intTree.insert(4);

    std::cout << "Int tree (in-order): ";
    printVector(intTree.inOrder());
    std::cout << "Contains 4? " << (intTree.contains(4) ? "yes" : "no") << "\n";
    std::cout << "Contains 9? " << (intTree.contains(9) ? "yes" : "no") << "\n\n";

    BinaryTree<std::string> stringTree;
    stringTree.insert("pear");
    stringTree.insert("apple");
    stringTree.insert("orange");
    stringTree.insert("banana");

    std::cout << "String tree (in-order): ";
    printVector(stringTree.inOrder());
    std::cout << "Contains \"banana\"? " << (stringTree.contains("banana") ? "yes" : "no") << "\n";
    std::cout << "Contains \"grape\"? " << (stringTree.contains("grape") ? "yes" : "no") << "\n";

    return 0;
}
