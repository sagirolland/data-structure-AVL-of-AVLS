#include "dspotify25b1.h"
#include "AVL.h"

template <class T>
AVLNode<T> *AVL<T>::new_node(T data, int info)
{
    AVLNode<T> *node = new AVLNode<T>(data, info);
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    if (data <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (info < 0)
    {
        return StatusType::INVALID_INPUT;
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
StatusType AVL<T>::rotate_left(AVLNode<T> *node)
{
    AVLNode<T> *Right = node->right;
    if (Right == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    node->right = Right->left;
    if (Right->left != nullptr)
    {
        Right->left->parent = node;
    }
    Right->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = Right;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = Right;
    }
    else
    {
        node->parent->right = Right;
    }
    Right->left = node;
    node->parent = Right;
    update_height(node);
    update_height(Right);
}

template <class T>
StatusType AVL<T>::rotate_right(AVLNode<T> *node)
{
    AVLNode<T> *Left = node->left;
    if (Left == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    node->left = Left->right;
    if (Left->right != nullptr)
    {
        Left->right->parent = node;
    }
    Left->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = Left;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = Left;
    }
    else
    {
        node->parent->left = Left;
    }
    Left->right = node;
    node->parent = Left;
    update_height(node);
    update_height(Left);
}

template <class T>
StatusType AVL<T>::rotate_RL(AVLNode<T> *node)
{
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    rotate_right(node->right);
    rotate_left(node);
}

template <class T>
StatusType AVL<T>::rotate_RR(AVLNode<T> *node)
{
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    rotate_left(node);
}

template <class T>
StatusType AVL<T>::rotate_LR(AVLNode<T> *node)
{
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    rotate_left(node->left);
    rotate_right(node);
}

template <class T>
StatusType AVL<T>::rotate_LL(AVLNode<T> *node)
{
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    rotate_right(node);
}

template <class T>
StatusType AVL<T>::rebalance(AVLNode<T> *node)
{
    AVLNode<T> *current = node;
    while (current != nullptr)
    {
        update_height(current);
        int bf = current->bf;

        if (bf == 2)
        {
            if (current->left->bf = > 0)
            {
                rotate_LL(current);
            }
            else
            {
                rotate_LR(current);
            }
        }
        else if (current->bf == -2)
        {
            if (current->right->bf <= 0)
            {
                rotate_RR(parent);
            }
            else
            {
                rotate_RL(parent);
            }
        }
        else
        {
            current = current->parent;
        }
    }
}

template <class T>
StatusType AVL<T>::insert(T data, int info)
{
    AVLNode<T> *pointer = root;
    if (data <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (info < 0)
    {
        return StatusType::INVALID_INPUT;
    }

    AVLNode<T> *node = new_node(data, info);
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    if (find(data) == StatusType::SUCCESS)
    {
        return StatusType::FAILURE;
    }

    if (root == nullptr)
    {
        root = node;
        return StatusType::SUCCESS;
    }

    while (1)
    {
        if (node->data < pointer->data)
        {
            if (pointer->left == nullptr)
            {
                pointer->left = node;
                node->parent = pointer;
                break;
            }

            else
            {
                pointer = pointer->left;
            }
        }
        else if (node->data > pointer->data)
        {
            if (pointer->right == nullptr)
            {
                pointer->right = node;
                node->parent = pointer;
                break;
            }
            else
            {
                pointer = pointer->right;
            }
        }
        else
        {
            delete node;
            return StatusType::FAILURE;
        }
    }

    rebalance(node);

    return StatusType::SUCCESS;
}

template <class T>
StatusType AVL<T>::find(T data)
{
    AVLNode<T> *node = root;
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    while (node != nullptr)
    {
        if (node->data == data)
        {
            return StatusType::SUCCESS;
        }

        if (node->data > data)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }

    return StatusType::FAILURE;
}

template <class T>
StatusType AVL<T>::remove(T data)
{

}

template <class T>
T* AVL<T>::inorder(AVLNode<T> *root, T *array)
{
    if (root == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    if (root->left != nullptr)
    {
        inorder(root->left, array);
    }
    array[root->index] = root->data;
    if (root->right != nullptr)
    {
        inorder(root->right, array);
    }
}