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

    StatusType *insert( T data, int info);

    StatusType *remove( T data);

    StatusType *find( T data);

    void rotate_left(AVLNode<T>* node);

    void rotate_right(AVLNode<T>* node);
    void rotate_RL(AVLNode<T>* node);
    void rotate_RR(AVLNode<T>* node);
    void rotate_LR(AVLNode<T>* node);
    void rotate_LL(AVLNode<T>* node);
};

template <class T>
class Playlist: public AVL<T>
{
private:
    AVLNode<T> *root;
    Playlist* next;
public: 
    Playlist(T ID){
        Playlist* new_playlist = new Playlist();
        new_playlist->root = this->new_node(ID, 1);
    }

    ~Playlist() {
        delete root;
    }
};

template <class T>
class Song: public AVL<T>
{
private:
    AVLNode<T> *root;
    Song* next;
public:
    Song(T ID, int plays){
        Song* new_song = new Song();
        new_song->root = this->new_node(ID, plays);
    }

    ~Song() {
        delete root;
    }
};
#endif // AVL.h