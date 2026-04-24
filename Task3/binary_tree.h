#pragma once

#include <vector>

template <typename T>
class BinaryTree {
private:
    struct Node {
        T value;
        Node* left;
        Node* right;

        explicit Node(const T& val);
    };

    Node* root;

    Node* insert(Node* node, const T& value);
    bool contains(Node* node, const T& value) const;
    void inOrder(Node* node, std::vector<T>& result) const;
    void clear(Node* node);

public:
    BinaryTree();
    BinaryTree(const BinaryTree& other);
    BinaryTree(BinaryTree&& other) noexcept;
    BinaryTree& operator=(const BinaryTree& other);
    BinaryTree& operator=(BinaryTree&& other) noexcept;
    ~BinaryTree();

    void insert(const T& value);
    bool contains(const T& value) const;
    std::vector<T> inOrder() const;

private:
    Node* clone(Node* node) const;
};

#include "binary_tree.hpp"
