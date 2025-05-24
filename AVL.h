#ifndef AVL_H_
#define AVL_H_


class Song {
public:
  int plays;
  int playlist_refers = 0;

  Song(int plays) : plays(plays), playlist_refers(0) {}
  ~Song() {}

  Song(const Song &) = delete;
  Song &operator=(const Song &) = delete;
};


template <class T> struct AVLNode {
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

  void update_height(AVLNode<T> *node) {
    if (node == nullptr) {
      return;
    }
    node->height_left = get_height(node->left);
    node->height_right = get_height(node->right);
    node->bf = get_bf(node);
  }

  T get_data(AVLNode<T> *node) { return node->data; }

  int get_height(AVLNode<T> *node) {
    if (node == nullptr) {
      return 0;
    }
    if (node->height_left > node->height_right) {
      return node->height_left + 1;
    } else {
      return node->height_right + 1;
    }
  }
  int max(int a, int b) {
    if (a > b) {
      return a;
    } else {
      return b;
    }
  }
  int get_bf(AVLNode<T> *node) {
    if (node == nullptr) {
      return 0;
    }
    return get_height(node->left) - get_height(node->right);
  }
};

template <class T> class AVL {
private:
  AVLNode<T> *root;

public:
  AVL() { root = nullptr; }

  virtual ~AVL() { destroy(root); }

  void destroy(AVLNode<T> *node) {
    if (node == nullptr) {
      return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
  }

  AVLNode<T> *new_node(T data, int uid) {
    AVLNode<T> *node = new AVLNode<T>(data, uid);
    if (node == nullptr) {
      return nullptr;
    }

    return node;
  }

  AVLNode<T> *get_root() { return root; }
  void set_root(AVLNode<T> *node) { root = node; }

  AVLNode<T> *rebalance(AVLNode<T> *node) {
    AVLNode<T> *current = node;
    while (current != nullptr) {

      current->update_height(current);
      int bf = current->bf;

      if (bf == 2) {
        if (current->left && current->left->bf >= 0) {
          current = rotate_LL(current);
        } else if (current->left) {
          current = rotate_LR(current);
        }
      } else if (bf == -2) {
        if (current->right && current->right->bf <= 0) {
          current = rotate_RR(current);
        } else if (current->right) {
          current = rotate_RL(current);
        }
      }

      current = current->parent;
    }
    return node;
  }

  int insert(T data, int uid) {
    AVLNode<T> *pointer = root;
    AVLNode<T> *node = new_node(data, uid);

    if (node == nullptr) {

      return 0;
    }

    if (root == nullptr) {

      root = node;
      return 1;
    }

    AVLNode<T> *search = findwithint(uid);

    if (search != nullptr) {

      return 0;
    }


    while (1) {
      if (node->uid < pointer->uid) {
        if (pointer->left == nullptr) {
          pointer->left = node;
          node->parent = pointer;
          break;
        }

        else {
          pointer = pointer->left;
        }
      } else if (node->uid > pointer->uid) {
        if (pointer->right == nullptr) {
          pointer->right = node;
          node->parent = pointer;
          break;
        } else {
          pointer = pointer->right;
        }
      } else {
        delete node;
        return 0;
      }
    }

    rebalance(node);

    return 1;
  }

  int remove(int uid) {

      AVLNode<T> *node = findwithint(uid);

      if (node == nullptr)
      {

          return 0;
      }
    AVLNode<T> *parent = node->parent;

    if (node->left == nullptr && node->right == nullptr) {
      if (parent) {
        if (parent->left == node) {
          parent->left = nullptr;
        } else if (parent->right == node) {
          parent->right = nullptr;
        }
      } else {
        root = nullptr;
      }

      delete node;
      rebalance(parent);
      return 1;
    }
    if (node->left == nullptr || node->right == nullptr) {

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
      } else {
        root = child;
      }
      child->parent = parent;
      delete node;
      rebalance(parent);
      return 1;
    }
    if (node->left != nullptr && node->right != nullptr) {
      AVLNode<T> *succ = node->right;
      while (succ->left) {
        succ = succ->left;
      }

      node->uid = succ->uid; // unclear. copy also data?
      node->data = succ->data;
      AVLNode<T> *succ_parent = succ->parent;
      AVLNode<T> *succ_child = (succ->right) ? succ->right : nullptr;

      if (succ_parent->left == succ)
      {
          succ_parent->left = succ_child;
      }
      else
      {
          succ_parent->right = succ_child;
      }

      if (succ_child)
      {
          succ_child->parent = succ_parent;
      }

      delete succ;
      rebalance(node);
      return 1;
    }

    return 0;
  }

  AVLNode<Song*> *find_by_info_ceiling(AVLNode<Song*> *node, int plays);

  AVLNode<T> *findwithint(int uid) {
    AVLNode<T> *node = root;

    if (node == nullptr) {

      return nullptr;
    }
  
    while (node != nullptr) {
      if (node->uid == uid) {

        return node;
      }

      if (node->uid > uid) {
        node = node->left;
      } else {
        node = node->right;
      }
    }

    return node;
  }

  AVLNode<T> *rotate_left(AVLNode<T> *node) {
    AVLNode<T> *Right = node->right;
    if (Right == nullptr) {
      return node;
    }
    node->right = Right->left;
    if (Right->left != nullptr) {
      Right->left->parent = node;
    }
    Right->parent = node->parent;
    if (node->parent == nullptr) {
      root = Right;
    } else if (node == node->parent->left) {
      node->parent->left = Right;
    } else {
      node->parent->right = Right;
    }
    Right->left = node;
    node->parent = Right;
    node->update_height(node);
    Right->update_height(Right);
    return Right;
  }
  AVLNode<T> *rotate_right(AVLNode<T> *node) {
    AVLNode<T> *Left = node->left;
    if (Left == nullptr) {
      return node;
    }
    node->left = Left->right;
    if (Left->right != nullptr) {
      Left->right->parent = node;
    }
    Left->parent = node->parent;
    if (node->parent == nullptr) {
      root = Left;
    } else if (node == node->parent->right) {
      node->parent->right = Left;
    } else {
      node->parent->left = Left;
    }
    Left->right = node;
    node->parent = Left;
    node->update_height(node);
    Left->update_height(Left);
    return Left;
  }
  AVLNode<T> *rotate_RL(AVLNode<T> *node) {
    if (node == nullptr) {
      return nullptr;
    }
    node->right = rotate_right(node->right);
    if (node->right) {
      node->right->parent = node;
    }
    return rotate_left(node);
  }
  AVLNode<T> *rotate_RR(AVLNode<T> *node) {
    if (node == nullptr) {
      return nullptr;
    }

    return rotate_left(node);
  }
  AVLNode<T> *rotate_LR(AVLNode<T> *node) {
    if (node == nullptr) {
      return nullptr;
    }
    node->left = rotate_left(node->left);
    if (node->left) {
      node->left->parent = node;
    }
    return rotate_right(node);
  }
  AVLNode<T> *rotate_LL(AVLNode<T> *node) {
    if (node == nullptr) {
      return nullptr;
    }

    return rotate_right(node);
  }

  // Static inorder traversal with callback and context
  static void inorder_with_context(AVLNode<T> *node, void (*cb)(AVLNode<T> *, void *), void *ctx)
  {
      if (!node)
          return;
      inorder_with_context(node->left, cb, ctx);
      cb(node, ctx);
      inorder_with_context(node->right, cb, ctx);
  }
};

struct PlaysSearchContext
{
    int plays;
    int best_id;
    int best_plays;
    bool found;
};

static void find_by_plays_cb(AVLNode<Song *> *node, void *ctx)
{
    PlaysSearchContext *context = (PlaysSearchContext *)ctx;
    int song_plays = node->data->plays;
    int song_id = node->uid;
    if (song_plays >= context->plays)
    {
        if (!context->found || song_plays < context->best_plays ||
            (song_plays == context->best_plays && song_id < context->best_id))
        {
            context->best_plays = song_plays;
            context->best_id = song_id;
            context->found = true;
        }
    }
    // Do NOT update anything if song_plays < context->plays!
}

template <class T>
AVLNode<Song*> *AVL<T>::find_by_info_ceiling(AVLNode<Song*> *node, int plays) {
  AVLNode<Song*> *res = nullptr;
  while (node != nullptr)
  {
      if (node->data->plays == plays)
      {
          return node;
      }
      if (node->data->plays < plays)
      {
          res = node;
          node = node->left;
      }
      else
      {
          node = node->right;
      }
  }
  return node;
}

class Playlist
{
public:
    AVL<Song *> *playlist_song_tree_root;
    int size = 0;

    Playlist() : playlist_song_tree_root(new AVL<Song *>()) {}

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
struct linkedListNode {
  AVLNode<T>* songlistNode;
  linkedListNode *next;

  linkedListNode(AVLNode<T>* songlistNode) : songlistNode(songlistNode), next(nullptr) {}
};
template <class T>
class mergeAVLTools
{
public:
   static int count_nodes(AVLNode<Song *> *node)
    {
        if (!node)
            return 0;
        return 1 + count_nodes(node->left) + count_nodes(node->right);
    }

    static void tree2list(AVLNode<T> *root,
                          linkedListNode<T> *&head,
                          linkedListNode<T> *&tail)
    {
        if (!root)
            return;

        tree2list(root->left, head, tail);

        linkedListNode<T> *new_node = new linkedListNode<T>(root);
        if (!head)
        {
            head = tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }

        tree2list(root->right, head, tail);
    }

    static AVLNode<T> *list2tree(linkedListNode<T> *&head, int n)
    {
        if (n <= 0 || !head)
            return nullptr;

        AVLNode<T> *left = list2tree(head, n / 2);

        // Create new AVL node using song and uid
        AVLNode<T> *root = new AVLNode<T>(head->songlistNode->data, head->songlistNode->uid);
        root->left = left;
        if (left)
            left->parent = root;

        head = head->next;

        AVLNode<T> *right = list2tree(head, n - n / 2 - 1);
        root->right = right;
        if (right)
            right->parent = root;

        root->update_height(root); // Update height and size if needed
        return root;
    }

    static void add_to_list(linkedListNode<T> *&head,
                            linkedListNode<T> *&tail,
                            linkedListNode<T> *node)
    {
        if (!head)
        {
            head = tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
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

    static AVLNode<T> *mergeTrees(AVLNode<T> *root1, AVLNode<T> *root2)
    {
        linkedListNode<T> *head1 = nullptr;
        linkedListNode<T> *tail1 = nullptr;
        linkedListNode<T> *head2 = nullptr;
        linkedListNode<T> *tail2 = nullptr;

        tree2list(root1, head1, tail1);
        tree2list(root2, head2, tail2);

        linkedListNode<T> *merged_head = nullptr;
        linkedListNode<T> *merged_tail = nullptr;

        while (head1 && head2)
        {
            if (head1->songlistNode->uid < head2->songlistNode->uid)
            {
                auto *node = new linkedListNode<T>(head1->songlistNode);
                add_to_list(merged_head, merged_tail, node);
                head1 = head1->next;
            }
            else if (head1->songlistNode->uid > head2->songlistNode->uid)
            {
                auto *node = new linkedListNode<T>(head2->songlistNode);
                add_to_list(merged_head, merged_tail, node);
                head2 = head2->next;
            }
            else
            {
                // Same uid → use one and decrement playlist reference
                auto *node = new linkedListNode<T>(head1->songlistNode);
                add_to_list(merged_head, merged_tail, node);
                node->songlistNode->data->playlist_refers--; // Handle duplicates
                head1 = head1->next;
                head2 = head2->next;
            }
        }

        // Append remaining
        while (head1)
        {
            auto *node = new linkedListNode<T>(head1->songlistNode);
            add_to_list(merged_head, merged_tail, node);
            head1 = head1->next;
        }
        while (head2)
        {
            auto *node = new linkedListNode<T>(head2->songlistNode);
            add_to_list(merged_head, merged_tail, node);
            head2 = head2->next;
        }

        int total_size = listSize(merged_head);
        return list2tree(merged_head, total_size);
    }
};

#endif // AVL.h
