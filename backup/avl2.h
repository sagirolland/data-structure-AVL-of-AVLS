#ifndef AVL_H_
#define AVL_H_
#include <iostream>

using namespace std;

class Song
{
public:
    int plays;
    int playlist_refers = 0;

    Song(int plays) : plays(plays), playlist_refers(0) {}
    ~Song() {}

    Song(const Song &) = delete;
    Song &operator=(const Song &) = delete;
};

class Playlist
{
public:
    AVL<Song *> *playlist_song_tree_root;
    int size = 0;

    Playlist() : playlist_song_tree_root(new AVL<Song *>()), size(0) {}

    ~Playlist()
    {
        delete playlist_song_tree_root;
        playlist_song_tree_root = nullptr;
    }
    // bool operator>(const Playlist *other) const { return id > other->id; }

    // bool operator<=(const Playlist *other) const { return id <= other->id; }

    // bool operator>=(const Playlist *other) const { return id >= other->id; }

    // bool operator<(const Playlist *other) const { return id < other->id; }
    // bool operator==(const Playlist *other) const { return id == other->id; }

    AVL<Song *> *get_songs_tree() const { return playlist_song_tree_root; }

    void set_songs_tree(AVL<Song *> *node) { playlist_song_tree_root = node; }

    Playlist(const Playlist &) = delete;
    Playlist &operator=(const Playlist &) = delete;
};

template <class T>
struct AVLNode
{
    int uid;
    T data;
    AVLNode<T> *left;
    AVLNode<T> *right;
    AVLNode<T> *parent;
    int height_left;
    int height_right;
    int bf = height_left - height_right;

    AVLNode(T data, int uid)
        : data(data), uid(uid), left(nullptr), right(nullptr), parent(nullptr),
          height_left(0), height_right(0) {}

    ~AVLNode() {}

    void update_height(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return;
        }
        node->height_left = get_height(node->left);
        node->height_right = get_height(node->right);
        node->bf = get_bf(node);
    }

    T get_data(AVLNode<T> *node) { return node->data; }

    int get_height(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return 0;
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
            return 0;
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
    AVL() { root = nullptr; }

    virtual ~AVL() { destroy(root); }

    void destroy(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    AVLNode<T> *new_node(T data, int uid)
    {
        AVLNode<T> *node = new AVLNode<T>(data, uid);
        if (node == nullptr)
        {
            return nullptr;
        }

        return node;
    }

    AVLNode<T> *get_root() { return root; }
    void set_root(AVLNode<T> *node) { root = node; }

    AVLNode<T> *rebalance(AVLNode<T> *node)
    {
        AVLNode<T> *current = node;
        while (current != nullptr)
        {

            current->update_height(current);
            int bf = current->bf;

            if (bf == 2)
            {
                if (current->left && current->left->bf >= 0)
                {
                    current = rotate_LL(current);
                }
                else if (current->left)
                {
                    current = rotate_LR(current);
                }
            }
            else if (bf == -2)
            {
                if (current->right && current->right->bf <= 0)
                {
                    current = rotate_RR(current);
                }
                else if (current->right)
                {
                    current = rotate_RL(current);
                }
            }

            current = current->parent;
        }
        return node;
    }

    int insert(T data, int uid)
    {
        AVLNode<T> *pointer = root;
        AVLNode<T> *node = new_node(data, uid);
        std ::cout << "insert 1 " << std::endl;

        if (node == nullptr)
        {
            std ::cout << "insert 2 " << std::endl;

            return 0;
        }
        std ::cout << "insert 3 " << std::endl;

        if (root == nullptr)
        {
            std ::cout << "insert 4 " << std::endl;

            root = node;
            return 1;
        }

        AVLNode<T> *search = find(data);
        std ::cout << "insert 5 " << std::endl;

        if (search != nullptr)
        {
            std ::cout << "insert 6 " << std::endl;

            return 0;
        }

        std ::cout << "insert 7 inorder " << std::endl;

        while (1)
        {
            if (node->uid < pointer->uid)
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
            else if (node->uid > pointer->uid)
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
                return 0;
            }
        }

        rebalance(node);
        std ::cout << "insert 8 " << std::endl;

        return 1;
    }

    int remove(int uid)
    {
        AVLNode<T> *node = find(uid);
        if (node == nullptr)
        {
            return 0;
        }
        AVLNode<T> *parent = node->parent;

        if (node->left == nullptr && node->right == nullptr)
        {
            if (parent)
            {
                if (parent->left == node)
                {
                    parent->left = nullptr;
                }
                else if (parent->right == node)
                {
                    parent->right = nullptr;
                }
            }
            else
            {
                root = nullptr;
            }
            delete node;
            rebalance(parent);
            return 1;
        }

        if (node->left == nullptr || node->right == nullptr)
        {

            AVLNode<T> *child = (node->left) ? node->left : node->right;
            if (parent)
            {
                if (parent->left == node)
                {
                    parent->left = child;
                }
                else
                {
                    parent->right = child;
                }
            }
            else
            {
                root = child;
            }
            child->parent = parent;
            delete node;
            rebalance(parent);
            return 1;
        }
        if (node->left != nullptr && node->right != nullptr)
        {
            AVLNode<T> *succ = node->right;
            while (succ->left)
            {
                succ = succ->left;
            }

            node->uid = succ->uid; // unclear. copy also data?
            remove(succ->uid);
            rebalance(node);
            return 1;
        }
        return 0;
    }

    AVLNode<T> *find(int uid)
    {
        AVLNode<T> *node = root;
        std ::cout << "find 1 " << std::endl;

        if (node == nullptr)
        {
            std ::cout << "find 2 " << std::endl;

            return nullptr;
        }
        std ::cout << "find 3 " << std::endl;
        std::cout << "find 3-1 " << node->uid << std::endl;
        std ::cout << "find 3-2" << data << std::endl;
        while (node != nullptr)
        {
            if (node->uid == uid)
            {
                std ::cout << "find 3-2" << node->uid << std::endl;

                return node;
            }

            if (node->uid > uid)
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }
        std ::cout << "find 4 " << std::endl;

        return node;
    }
    AVLNode<T> *find_by_info_ceiling(AVLNode<T> *node, int plays);

    AVLNode<T> *findwithint(int uid)
    {
        AVLNode<T> *node = root;
        std ::cout << "find 1 " << std::endl;

        if (node == nullptr)
        {
            std ::cout << "find 2 " << std::endl;

            return nullptr;
        }
        std ::cout << "find 3 " << std::endl;
        std::cout << "find 3-1 " << node->uid << std::endl;
        std ::cout << "find 3-2" << uid << std::endl;
        while (node != nullptr)
        {
            if (node->uid == uid)
            {
                std ::cout << "find 3-2" << node->uid << std::endl;

                return node;
            }

            if (node->uid > uid)
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }
        std ::cout << "find 4 " << std::endl;

        return node;
    }

    AVLNode<T> *rotate_left(AVLNode<T> *node)
    {
        AVLNode<T> *Right = node->right;
        if (Right == nullptr)
        {
            return node;
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
        node->update_height(node);
        Right->update_height(Right);
        return Right;
    }
    AVLNode<T> *rotate_right(AVLNode<T> *node)
    {
        AVLNode<T> *Left = node->left;
        if (Left == nullptr)
        {
            return node;
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
        node->update_height(node);
        Left->update_height(Left);
        return Left;
    }
    AVLNode<T> *rotate_RL(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        node->right = rotate_right(node->right);
        if (node->right)
        {
            node->right->parent = node;
        }
        return rotate_left(node);
    }
    AVLNode<T> *rotate_RR(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        return rotate_left(node);
    }
    AVLNode<T> *rotate_LR(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        node->left = rotate_left(node->left);
        if (node->left)
        {
            node->left->parent = node;
        }
        return rotate_right(node);
    }
    AVLNode<T> *rotate_LL(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        return rotate_right(node);
    }
};

template <class T>
AVLNode<T> *AVL<T>::find_by_info_ceiling(AVLNode<T> *node, int plays)
{
    // AVLNode<T> *res = nullptr;
    // while (node != nullptr)
    // {
    //     if (node->data == plays)
    //     {
    //         return node;
    //     }
    //     if (node->data < plays)
    //     {
    //         res = node;
    //         node = node->left;
    //     }
    //     else
    //     {
    //         node = node->right;
    //     }
    // }
    return 1;
}

template <class T>
struct linkedListNode
{
    T data;
    linkedListNode *next;

    linkedListNode(T data) : data(data), next(nullptr) {}
};

template <class T>
class mergeAVLTools
{
public:
    static void tree2list(AVLNode<T> *node, linkedListNode<T> *&head,
                          linkedListNode<T> *&tail)
    {
        if (node == nullptr)
        {
            return;
        }
        tree2list(node->left, head, tail);
        linkedListNode<T> *new_node = new linkedListNode<T>(node->data);
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }
        tree2list(node->right, head, tail);
    }
    static int listSize(linkedListNode<T> *head)
    {
        int size = 0;
        while (head)
        {
            size++;
            head = head->next;
        }
        return size;
    }
    static AVLNode<T> *list2tree(linkedListNode<T> *&head, int size)
    {
        if (size <= 0 || head == nullptr)
        {
            return nullptr;
        }
        AVLNode<T> *left = list2tree(head, size / 2);
        AVLNode<T> *root = new AVLNode<T>(head->uid, 0);
        head = head->next;
        AVLNode<T> *right = list2tree(head, size - size / 2 - 1);
        root->left = left;
        root->right = right;
        root->update_height(root);
        return root;
    }
    static void add_to_list(linkedListNode<T> *&head, linkedListNode<T> *&tail,
                            linkedListNode<T> *&node)
    {
        if (head == nullptr)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
    }
};

#endif // AVL.h
