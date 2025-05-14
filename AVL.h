#ifndef AVL_H_
#define AVL_H_

#include "dspotify25b1.h"
template<class T>
struct AVLNode
{
    T data;
    int info=0;
    AVLNode* left;
    AVLNode* right;
    int height_left;
    int height_right;
    int bf = height_left - height_right;
    AVLNode* parent;
    AVLNode* shared_ptr = nullptr;
    int index = 0;

    AVLNode(T data,int info) : data(data), left(nullptr), right(nullptr), height_left(0),
    height_right(0), parent(nullptr), info(info)
    {}

    ~AVLNode() {
        delete left;
        delete right;
    }
};

template<class T>
class AVL
{
private:
    AVLNode<T> *root;
public:
    AVL(){root = nullptr;};

    virtual ~AVL(); //postorder delete

    AVLNode<T>* new_node(T data, int info);

    AVLNode<T>* get_root() { return root; }

    T* inorder(AVLNode<T>* root, T* array);

    T get_data(AVLNode<T>* node) { return node->data; }

    int get_info(AVLNode<T>* node) { return node->info; }

    AVLNode<T> *insert(AVLNode<T> *node, T data, int info);

    AVLNode<T> *remove(AVLNode<T> *node, T data, int info);

    AVLNode<T> *find(AVLNode<T> *node, T data, int info);
};



#endif // AVL.h