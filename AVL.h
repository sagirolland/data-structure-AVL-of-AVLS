#ifndef AVL_H_
#define AVL_H_

#include "dspotify25b1.h"
template <class T>
struct AVLNode
{
    T data;
    int info = 0;
    AVLNode *left;
    AVLNode *right;
    int height_left;
    int height_right;
    int bf = height_left - height_right;
    AVLNode *parent;
    AVLNode *shared_ptr = nullptr;
    int index = 0;

    AVLNode(T data, int info) : data(data), left(nullptr), right(nullptr), height_left(0),
                                height_right(0), parent(nullptr), info(info)
    {
    }

    ~AVLNode()
    {
        if (left != nullptr)
        {
            delete left;
        }
        if (right != nullptr)
        {
            delete right;
        }
        delete this;
    }

    T get_data(AVLNode<T> *node) { return node->data; }

    int get_info(AVLNode<T> *node) { return node->info; }

    int get_height(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return 1;
        }
        if (node->height_left > node->height_right)
        {
            return node->height_left + 1;
        }
        else
        {
            return node->height_right + 1;
        }
    }
    int max(int a, int b)
    {
        if (a > b)
        {
            return a;
        }
        else
        {
            return b;
        }
    }
    int get_bf(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return 1;
        }
        return get_height(node->left) - get_height(node->right);
    }
};

template <class T>
class AVL
{
private:
    AVLNode<T> *root;

public:
    AVL() { root = nullptr; };

    virtual ~AVL();

    void destroy(AVLNode<T> *node);

    AVLNode<T> *new_node(T data, int info);

    AVLNode<T> *get_root() { return root; }

    T *inorder(AVLNode<T> *root, T *array);

    AVLNode<T> rebalance(AVLNode<T> *node);

    StatusType insert(T data, int info);

    StatusType remove(T data);

    AVLNode<T> *find(T data);

    AVLNode<T> rotate_left(AVLNode<T> *node);

    AVLNode<T> rotate_right(AVLNode<T> *node);
    AVLNode<T> rotate_RL(AVLNode<T> *node);
    AVLNode<T> rotate_RR(AVLNode<T> *node);
    AVLNode<T> rotate_LR(AVLNode<T> *node);
    AVLNode<T> rotate_LL(AVLNode<T> *node);
    void update_height(AVLNode<T> *node);
};

template <class T>
class Playlist : public AVL<T>
{
private:
    AVLNode<T> *root;

public:
    Playlist(T ID)
    {
        Playlist *new_playlist = new AVL();
        new_playlist->root = this->new_node(ID, 1);
    }
    void destroy(root){AVL::destroy(root);}
     ~Playlist()
    {
        if (root == nullptr)
        {
            return;
        }
        if (root->left != nullptr)
        {
            ~AVL(root->left);
        }
        delete if (root->right != nullptr)
        {
            ~AVL(root->right);
        }
    }
};

template <class T>
class Song : public AVL<T>
{
private:
    AVLNode<T> *root;
public:
    Song(T ID, int plays)
    {
        Song *new_song = new AVL();
        new_song->root = this->new_node(ID, plays);
    }
    void destroy(root) { AVL::destroy(root); }
    ~Song()
    {
        if (root == nullptr)
        {
            return;
        }
        if (root->left != nullptr)
        {
            ~AVL(root->left);
        }
        delete if (root->right != nullptr)
        {
            ~AVL(root->right);
        }
    }
};
#endif // AVL.h