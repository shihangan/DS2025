#ifndef MYSTL_TREE_H
#define MYSTL_TREE_H

#include <cstddef>
#include "vector.h"

namespace MySTL {

template <typename T>
struct TreeNode {
    T data;
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
    TreeNode(const T& v, TreeNode* p = nullptr) : data(v), parent(p), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree {
private:
    TreeNode<T>* _root;
    int _size;
    void release(TreeNode<T>* x);
    int count(TreeNode<T>* x);

public:
    BinaryTree();
    ~BinaryTree();
    int size() const;
    bool empty() const;
    TreeNode<T>* root() const;
    TreeNode<T>* insertAsRoot(const T& e);
    TreeNode<T>* insertAsLeft(TreeNode<T>* x, const T& e);
    TreeNode<T>* insertAsRight(TreeNode<T>* x, const T& e);
    void attachLeft(TreeNode<T>* x, BinaryTree<T>& t);
    void attachRight(TreeNode<T>* x, BinaryTree<T>& t);
    int remove(TreeNode<T>* x);
    void clear();
    TreeNode<T>* find(const T& e) const;
    template <typename VST> void preorder(VST& visit) const;
    template <typename VST> void inorder(VST& visit) const;
    template <typename VST> void postorder(VST& visit) const;
    template <typename VST> void levelorder(VST& visit) const;
};

} // namespace MySTL

namespace MySTL {

template <typename T, int TDeg>
struct BNode {
    Vector<T> keys;
    Vector<BNode*> child;
    bool leaf;
    BNode(bool isLeaf = true) : keys(), child(), leaf(isLeaf) {}
};

template <typename T, int TDeg>
class BTree {
private:
    BNode<T, TDeg>* _root;
    int _size;
    void clearNode(BNode<T, TDeg>* x);
    BNode<T, TDeg>* searchNode(BNode<T, TDeg>* x, const T& e) const;
    void splitChild(BNode<T, TDeg>* x, int i);
    void insertNonFull(BNode<T, TDeg>* x, const T& e);

public:
    BTree();
    ~BTree();
    int size() const;
    bool empty() const;
    BNode<T, TDeg>* root() const;
    bool contains(const T& e) const;
    void insert(const T& e);
    void clear();
};

} // namespace MySTL

namespace MySTL {

template <typename T>
struct AVLNode {
    T data;
    AVLNode* parent;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(const T& v, AVLNode* p = nullptr) : data(v), parent(p), left(nullptr), right(nullptr), height(1) {}
};

template <typename T>
class AVLTree {
private:
    AVLNode<T>* _root;
    int _size;
    void clearNode(AVLNode<T>* x);
    int h(AVLNode<T>* x) const;
    void update(AVLNode<T>* x);
    AVLNode<T>* rotateLeft(AVLNode<T>* x);
    AVLNode<T>* rotateRight(AVLNode<T>* x);
    void rebalance(AVLNode<T>* x);

public:
    AVLTree();
    ~AVLTree();
    int size() const;
    bool empty() const;
    bool contains(const T& e) const;
    void insert(const T& e);
    void clear();
    AVLNode<T>* root() const;
};

} // namespace MySTL

#include "tree.cpp"

#endif
