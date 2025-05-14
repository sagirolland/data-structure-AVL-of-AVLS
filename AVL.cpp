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
