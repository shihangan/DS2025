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

template <typename T, int TDeg>
BTree<T, TDeg>::BTree() : _root(nullptr), _size(0) {}

template <typename T, int TDeg>
BTree<T, TDeg>::~BTree() { clear(); }

template <typename T, int TDeg>
int BTree<T, TDeg>::size() const { return _size; }

template <typename T, int TDeg>
bool BTree<T, TDeg>::empty() const { return _root == nullptr; }

template <typename T, int TDeg>
BNode<T, TDeg>* BTree<T, TDeg>::root() const { return _root; }

template <typename T, int TDeg>
void BTree<T, TDeg>::clearNode(BNode<T, TDeg>* x) {
    if (!x) return;
    for (int i = 0; i < x->child.size(); ++i) clearNode(x->child[i]);
    delete x;
}

template <typename T, int TDeg>
void BTree<T, TDeg>::clear() { clearNode(_root); _root = nullptr; _size = 0; }

template <typename T, int TDeg>
BNode<T, TDeg>* BTree<T, TDeg>::searchNode(BNode<T, TDeg>* x, const T& e) const {
    if (!x) return nullptr;
    int i = 0;
    while (i < x->keys.size() && e > x->keys[i]) ++i;
    if (i < x->keys.size() && !(e < x->keys[i]) && !(x->keys[i] < e)) return x;
    if (x->leaf) return nullptr;
    return searchNode(x->child[i], e);
}

template <typename T, int TDeg>
bool BTree<T, TDeg>::contains(const T& e) const { return searchNode(_root, e) != nullptr; }

template <typename T, int TDeg>
void BTree<T, TDeg>::splitChild(BNode<T, TDeg>* x, int i) {
    BNode<T, TDeg>* y = x->child[i];
    BNode<T, TDeg>* z = new BNode<T, TDeg>(y->leaf);
    int maxKeys = 2 * TDeg - 1;
    int mid = TDeg - 1;
    for (int k = TDeg; k < y->keys.size(); ++k) z->keys.insert(z->keys.size(), y->keys[k]);
    if (!y->leaf) {
        for (int k = TDeg; k < y->child.size(); ++k) z->child.insert(z->child.size(), y->child[k]);
        y->child.remove(TDeg, y->child.size());
    }
    T median = y->keys[mid];
    y->keys.remove(TDeg, y->keys.size());
    y->keys.remove(mid);
    x->child.insert(i + 1, z);
    x->keys.insert(i, median);
}

template <typename T, int TDeg>
void BTree<T, TDeg>::insertNonFull(BNode<T, TDeg>* x, const T& e) {
    int i = x->keys.size() - 1;
    if (x->leaf) {
        int pos = x->keys.size();
        while (pos > 0 && e < x->keys[pos - 1]) --pos;
        x->keys.insert(pos, e);
        ++_size;
    } else {
        while (i >= 0 && e < x->keys[i]) --i;
        ++i;
        if (x->child[i]->keys.size() == 2 * TDeg - 1) {
            splitChild(x, i);
            if (e > x->keys[i]) ++i;
        }
        insertNonFull(x->child[i], e);
    }
}

template <typename T, int TDeg>
void BTree<T, TDeg>::insert(const T& e) {
    if (!_root) { _root = new BNode<T, TDeg>(true); insertNonFull(_root, e); return; }
    if (_root->keys.size() == 2 * TDeg - 1) {
        BNode<T, TDeg>* s = new BNode<T, TDeg>(false);
        s->child.insert(0, _root);
        splitChild(s, 0);
        _root = s;
        insertNonFull(_root, e);
    } else {
        insertNonFull(_root, e);
    }
}

template <typename T>
AVLTree<T>::AVLTree() : _root(nullptr), _size(0) {}

template <typename T>
AVLTree<T>::~AVLTree() { clear(); }

template <typename T>
int AVLTree<T>::size() const { return _size; }

template <typename T>
bool AVLTree<T>::empty() const { return _root == nullptr; }

template <typename T>
AVLNode<T>* AVLTree<T>::root() const { return _root; }

template <typename T>
void AVLTree<T>::clearNode(AVLNode<T>* x) {
    if (!x) return;
    clearNode(x->left);
    clearNode(x->right);
    delete x;
}

template <typename T>
void AVLTree<T>::clear() { clearNode(_root); _root = nullptr; _size = 0; }

template <typename T>
int AVLTree<T>::h(AVLNode<T>* x) const { return x ? x->height : 0; }

template <typename T>
void AVLTree<T>::update(AVLNode<T>* x) { if (x) { int hl = h(x->left), hr = h(x->right); x->height = (hl > hr ? hl : hr) + 1; } }

template <typename T>
AVLNode<T>* AVLTree<T>::rotateLeft(AVLNode<T>* x) {
    AVLNode<T>* y = x->right;
    x->right = y->left; if (y->left) y->left->parent = x;
    y->left = x; y->parent = x->parent; x->parent = y;
    if (!y->parent) _root = y; else if (y->parent->left == x) y->parent->left = y; else y->parent->right = y;
    update(x); update(y); return y;
}

template <typename T>
AVLNode<T>* AVLTree<T>::rotateRight(AVLNode<T>* x) {
    AVLNode<T>* y = x->left;
    x->left = y->right; if (y->right) y->right->parent = x;
    y->right = x; y->parent = x->parent; x->parent = y;
    if (!y->parent) _root = y; else if (y->parent->left == x) y->parent->left = y; else y->parent->right = y;
    update(x); update(y); return y;
}

template <typename T>
void AVLTree<T>::rebalance(AVLNode<T>* x) {
    while (x) {
        update(x);
        int bal = h(x->left) - h(x->right);
        if (bal > 1) {
            if (h(x->left->right) > h(x->left->left)) rotateLeft(x->left);
            x = rotateRight(x);
        } else if (bal < -1) {
            if (h(x->right->left) > h(x->right->right)) rotateRight(x->right);
            x = rotateLeft(x);
        }
        x = x->parent;
    }
}

template <typename T>
bool AVLTree<T>::contains(const T& e) const {
    AVLNode<T>* x = _root;
    while (x) {
        if (e < x->data) x = x->left;
        else if (x->data < e) x = x->right;
        else return true;
    }
    return false;
}

template <typename T>
void AVLTree<T>::insert(const T& e) {
    if (!_root) { _root = new AVLNode<T>(e); _size = 1; return; }
    AVLNode<T>* p = nullptr; AVLNode<T>* x = _root;
    while (x) { p = x; x = (e < x->data) ? x->left : x->right; }
    AVLNode<T>* z = new AVLNode<T>(e, p);
    if (e < p->data) p->left = z; else p->right = z;
    ++_size;
    rebalance(p);
}

} // namespace MySTL

#endif
