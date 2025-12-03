#ifndef MYSTL_TREE_H
#define MYSTL_TREE_H

#include <cstddef>

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

#include "tree.cpp"

#endif

