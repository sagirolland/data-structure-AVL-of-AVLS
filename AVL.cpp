#include "dspotify25b1.h"
#include "AVL.h"

template <class T>
AVLNode<T>* AVL<T>::new_node(T data, int info)
{
    AVLNode<T> *node = new AVLNode<T>(data, info);
    if (node == nullptr)
    {
        throw std::bad_alloc();
    }
    return node;
}

template <class T>
AVL<T>::~AVL()
{
    while (condition)
    {
        /* code */
    }
}

template <class T>
void AVL<T>::rotate_left(AVLNode<T>* node)
{
    AVLNode<T>* Right->right = node->right->left;
    node->right->parent = node->parent;
    node->parent = node->right;
    node->right=Right->right;
    node->parent->left = node;
}
template <class T>
void AVL<T>::rotate_right(AVLNode<T> *node)
{
}
template <class T>
void AVL<T>::rotate_RL(AVLNode<T> *node)
{
}
template <class T>
void AVL<T>::rotate_RR(AVLNode<T> *node)
{
}
template <class T>
void AVL<T>::rotate_LR(AVLNode<T> *node)
{
}
template <class T>
void AVL<T>::rotate_LL(AVLNode<T> *node)
{
}

template <class T>
StatusType *AVL<T>::insert(AVLNode<T>* node,T data, int info)
{
    if (node == nullptr)
    {
        return StatusType::FAILURE;
    }
    if(root->data < data)
    {
    
    }
    if (root->data > data)
    {
        
    }
}