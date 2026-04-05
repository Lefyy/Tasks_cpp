#pragma once

template <typename T>
BinaryTree<T>::Node::Node(const T& val) : value(val), left(nullptr), right(nullptr) {}

template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

template <typename T>
BinaryTree<T>::~BinaryTree() {
    clear(root);
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::insert(Node* node, const T& value) {
    if (!node) {
        return new Node(value);
    }

    if (value < node->value) {
        node->left = insert(node->left, value);
    } else {
        node->right = insert(node->right, value);
    }

    return node;
}

template <typename T>
bool BinaryTree<T>::contains(Node* node, const T& value) const {
    if (!node) {
        return false;
    }

    if (value == node->value) {
        return true;
    }

    if (value < node->value) {
        return contains(node->left, value);
    }

    return contains(node->right, value);
}

template <typename T>
void BinaryTree<T>::inOrder(Node* node, std::vector<T>& result) const {
    if (!node) {
        return;
    }

    inOrder(node->left, result);
    result.push_back(node->value);
    inOrder(node->right, result);
}

template <typename T>
void BinaryTree<T>::clear(Node* node) {
    if (!node) {
        return;
    }

    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T>
void BinaryTree<T>::insert(const T& value) {
    root = insert(root, value);
}

template <typename T>
bool BinaryTree<T>::contains(const T& value) const {
    return contains(root, value);
}

template <typename T>
std::vector<T> BinaryTree<T>::inOrder() const {
    std::vector<T> result;
    inOrder(root, result);
    return result;
}
