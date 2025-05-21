#ifndef AVL_H_
#define AVL_H_

template <class T>
struct AVLNode
{
    T data;
    int info = 0;
    int playlist_refers = 0;
    AVLNode<T> *left;
    AVLNode<T> *right;
    AVLNode<T> *parent;
    int height_left;
    int height_right;
    int bf = height_left - height_right;

    AVLNode(T data, int info) : data(data), info(info), left(nullptr), right(nullptr), parent(nullptr), height_left(0),
                                height_right(0)
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

    virtual ~AVL()
    {
        destroy(root);
    }

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

    AVLNode<T> *new_node(T data, int info)
    {
        AVLNode<T> *node = new AVLNode<T>(data, info);
        if (node == nullptr)
        {
            return nullptr;
        }

        return node;
    }

    AVLNode<T> *get_root() { return root; }
    void set_root(AVLNode<T> *node) { root = node; }
    
    AVLNode<T> rebalance(AVLNode<T> *node)
    {
        AVLNode<T> *current = node;
        while (current != nullptr)
        {
            current->update_height(current);
            int bf = current->bf;

            if (bf == 2)
            {
                if (current->left->bf && current->left >= 0)
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
                if (current->right->bf && current->right <= 0)
                {
                    rotate_RR(current);
                }
                else
                {
                    rotate_RL(current);
                }
            }
            else
            {
                current = current->parent;
            }
        }
        return *node;
    }

    int insert(T data, int info)
    {
        AVLNode<T> *pointer = root;
        AVLNode<T> *node = new_node(data, info);
        if (node == nullptr)
        {
            return 0;
        }
        AVLNode<T> *search = find(data);
        if (search != nullptr)
        {
            return 0;
        }

        if (root == nullptr)
        {
            root = node;
            return 1;
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
                return 0;
            }
        }

        rebalance(node);

        return 1;
    }

    int remove(T data)
    {
        AVLNode<T> *node = find(data);
        if (node == nullptr)
        {
            return 0;
        }
        if (node->left == nullptr && node->right == nullptr)
        {
            AVLNode<T> *parent = node->parent;
            delete node;
            rebalance(parent);
            return 0;
        }
        if (node->parent == nullptr)
        {
            delete node;
            return 0;
        }
        if (node->parent->left == node)
        {
            AVLNode<T> *parent = node->parent;
            if (node->left == nullptr)
            {
                node->right->parent = parent;
                parent->left = node->right;
            }
            if (node->right == nullptr)
            {
                node->left->parent = parent;
                parent->left = node->left;
            }
        }
        if (node->parent->right == node)
        {
            AVLNode<T> *parent = node->parent;
            if (node->left == nullptr)
            {
                node->right->parent = parent;
                parent->right = node->right;
            }
            if (node->right == nullptr)
            {
                node->left->parent = parent;
                parent->right = node->left;
            }
        }
        rebalance(node->parent);
        delete node;
        return 1;
    }

    AVLNode<T> *find(T data)
    {
        AVLNode<T> *node = root;
        if (node == nullptr)
        {
            return nullptr;
        }
        while (node != nullptr)
        {
            if (node->data == data)
            {
                return node;
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
        return node;
    }
    AVLNode<T> *find_by_info_ceiling(AVLNode<int> *node, int plays)
    {
        AVLNode<int> *res = nullptr;
        while (node != nullptr)
        {
            if (node->info == plays)
            {
                return node;
            }
            if (node->info > plays)
            {
                res = node;
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }
        return res;
    }

    AVLNode<T>* rotate_left(AVLNode<T> *node)
    {
        AVLNode<T> *Right = node->right;
        if (Right == nullptr)
        {
            return nullptr;
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
        return node;
    }
    AVLNode<T>* rotate_right(AVLNode<T> *node)
    {
        AVLNode<T> *Left = node->left;
        if (Left == nullptr)
        {
            return nullptr;
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
        return node;
    }
    AVLNode<T>* rotate_RL(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        rotate_right(node->right);
        rotate_left(node);
        return node;
    }
    AVLNode<T>* rotate_RR(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        rotate_left(node);
        return node;
    }
    AVLNode<T>* rotate_LR(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        rotate_left(node->left);
        rotate_right(node);
        return node;
    }
    AVLNode<T>* rotate_LL(AVLNode<T> *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        rotate_right(node);
        return node;}
};

template <class T>
class Playlist : public AVLNode<T>
{
private:
    T id;
    AVL<int> *playlist_root;

public:
    Playlist(T ID) : AVLNode<T>(ID, 0), id(ID), playlist_root(new AVL<T>()) {}
    ~Playlist()
    {
        delete playlist_root;
    }
    bool operator<(const Playlist &other) const
    {
        return id < other.id;
    }
    
    bool operator>(const Playlist &other) const
    {
        return id > other.id;
    }
    bool operator>=(const Playlist &other) const
    {
        return id >= other.id;
    }
    bool operator<=(const Playlist &other) const
    {
        return id <= other.id;
    }
    bool operator==(const Playlist &other) const
    {
        return id == other.id;
    }
    AVL<T> *get_songs_tree() const { return playlist_root; }
    void set_songs_tree(AVL<T> *node) { playlist_root = node; }
    T get_id() const { return id; }
};

template <class T>
class Song : public AVLNode<T>
{
private:
    AVL<T> *song_root;
    T id;
    int plays;

public:
    Song(T ID, int plays) : AVLNode<T>(ID, plays) {}
    ~Song()
    {
        delete song_root;
    }
    // < operator for get by plays
    bool operator<(const Song &other) const
    {
        return plays < other.plays;
    }
    bool operator>(const Song &other) const
    {
        return plays > other.plays;
    }
    bool operator==(const Song &other) const
    {
        return id == other.id && plays == other.plays;
    }
};

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
    static void tree2list(AVLNode<T> *node, linkedListNode<T> *&head, linkedListNode<T> *&tail)
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
    static AVLNode<T> *list2tree(linkedListNode<T> *head, int size)
    {
        if (size <= 0 || head == nullptr)
        {
            return nullptr;
        }
        AVLNode<T> *left = list2tree(head, size / 2);
        AVLNode<T> *root = new AVLNode<T>(head->data, 0);
        head = head->next;
        AVLNode<T> *right = list2tree(head, size - size / 2 - 1);
        root->left = left;
        root->right = right;
        root->update_height(root);
        return root;
    }
    static void add_to_list(linkedListNode<T> *&head, linkedListNode<T> *&tail, linkedListNode<T> *&node)
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
