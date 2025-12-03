#ifndef TREE_IMPLEMENTATION_INCLUDED
#define TREE_IMPLEMENTATION_INCLUDED

#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "tree.h"

namespace MySTL {

template <typename T>
BinaryTree<T>::BinaryTree() : _root(nullptr), _size(0) {}

template <typename T>
BinaryTree<T>::~BinaryTree() { clear(); }

template <typename T>
int BinaryTree<T>::size() const { return _size; }

template <typename T>
bool BinaryTree<T>::empty() const { return _size == 0; }

template <typename T>
TreeNode<T>* BinaryTree<T>::root() const { return _root; }

template <typename T>
void BinaryTree<T>::release(TreeNode<T>* x) {
    if (!x) return;
    release(x->left);
    release(x->right);
    delete x;
}

template <typename T>
int BinaryTree<T>::count(TreeNode<T>* x) {
    if (!x) return 0;
    return 1 + count(x->left) + count(x->right);
}

template <typename T>
TreeNode<T>* BinaryTree<T>::insertAsRoot(const T& e) {
    if (_root) return _root;
    _root = new TreeNode<T>(e);
    _size = 1;
    return _root;
}

template <typename T>
TreeNode<T>* BinaryTree<T>::insertAsLeft(TreeNode<T>* x, const T& e) {
    if (!x || x->left) return x ? x->left : nullptr;
    x->left = new TreeNode<T>(e, x);
    _size++;
    return x->left;
}

template <typename T>
TreeNode<T>* BinaryTree<T>::insertAsRight(TreeNode<T>* x, const T& e) {
    if (!x || x->right) return x ? x->right : nullptr;
    x->right = new TreeNode<T>(e, x);
    _size++;
    return x->right;
}

template <typename T>
void BinaryTree<T>::attachLeft(TreeNode<T>* x, BinaryTree<T>& t) {
    if (!x || x->left || !t._root) return;
    x->left = t._root;
    t._root->parent = x;
    _size += t._size;
    t._root = nullptr;
    t._size = 0;
}

template <typename T>
void BinaryTree<T>::attachRight(TreeNode<T>* x, BinaryTree<T>& t) {
    if (!x || x->right || !t._root) return;
    x->right = t._root;
    t._root->parent = x;
    _size += t._size;
    t._root = nullptr;
    t._size = 0;
}

template <typename T>
int BinaryTree<T>::remove(TreeNode<T>* x) {
    if (!x) return 0;
    if (x->parent) {
        if (x->parent->left == x) x->parent->left = nullptr;
        else if (x->parent->right == x) x->parent->right = nullptr;
    } else {
        _root = nullptr;
    }
    int k = count(x);
    release(x);
    _size -= k;
    return k;
}

template <typename T>
void BinaryTree<T>::clear() { remove(_root); }

template <typename T>
TreeNode<T>* BinaryTree<T>::find(const T& e) const {
    if (!_root) return nullptr;
    MySTL::Queue<TreeNode<T>*> q;
    q.push(_root);
    while (!q.empty()) {
        TreeNode<T>* u = q.pop();
        if (u->data == e) return u;
        if (u->left) q.push(u->left);
        if (u->right) q.push(u->right);
    }
    return nullptr;
}

template <typename T>
template <typename VST>
void BinaryTree<T>::preorder(VST& visit) const {
    if (!_root) return;
    Stack<TreeNode<T>*> s;
    TreeNode<T>* x = _root;
    while (x) {
        visit(x->data);
        if (x->right) s.push(x->right);
        x = x->left ? x->left : (s.empty() ? nullptr : s.pop());
    }
}

template <typename T>
template <typename VST>
void BinaryTree<T>::inorder(VST& visit) const {
    Stack<TreeNode<T>*> s;
    TreeNode<T>* x = _root;
    while (x || !s.empty()) {
        if (x) { s.push(x); x = x->left; }
        else { x = s.pop(); visit(x->data); x = x->right; }
    }
}

template <typename T>
template <typename VST>
void BinaryTree<T>::postorder(VST& visit) const {
    Stack<TreeNode<T>*> s;
    TreeNode<T>* x = _root;
    TreeNode<T>* last = nullptr;
    while (x || !s.empty()) {
        if (x) { s.push(x); x = x->left; }
        else {
            TreeNode<T>* node = s.top();
            if (node->right && last != node->right) { x = node->right; }
            else { visit(node->data); last = node; s.pop(); }
        }
    }
}

template <typename T>
template <typename VST>
void BinaryTree<T>::levelorder(VST& visit) const {
    if (!_root) return;
    MySTL::Queue<TreeNode<T>*> q;
    q.push(_root);
    while (!q.empty()) {
        TreeNode<T>* u = q.pop();
        visit(u->data);
        if (u->left) q.push(u->left);
        if (u->right) q.push(u->right);
    }
}

} // namespace MySTL

#endif

