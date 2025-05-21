// You can edit anything you want in this file.
// However you need to implement all public DSpotify function, as provided below as a template

#include "dspotify25b1.h"
#include "AVL.h"

DSpotify::DSpotify()
{
    song_root = new AVL<int>();
    playlist_root = new AVL<Playlist<int>>();
}

DSpotify::~DSpotify()
{
    delete song_root;
    delete playlist_root;
}

StatusType DSpotify::add_playlist(int playlistId)
{ // new avl root info = 1
    if (playlistId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (playlist_root->find(playlistId) != nullptr)
    {
        return StatusType::FAILURE;
    }

    Playlist<int> *node = new Playlist<int>(playlistId);

    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    int res = playlist_root->insert(playlistId, 0);
    if (res != 1)
    {
        delete node;
        return StatusType::FAILURE;
    }
    if (res == 1)
    {
        playlist_root->get_root()->info = 1;
    }
    return StatusType::SUCCESS;
}

StatusType DSpotify::delete_playlist(int playlistId)
{
    if (playlistId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLNode<Playlist<int>> *playlist = playlist_root->find(playlistId);
    if (playlist==nullptr){return StatusType::FAILURE;}
    playlist_root->remove(playlistId);
    delete playlist;
    return StatusType::SUCCESS;
}

StatusType DSpotify::add_song(int songId, int plays)
{
    if (songId <= 0 || plays < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (song_root->find(songId) != nullptr)
    {
        return StatusType::FAILURE;
    }

    Song<int> *node = new Song<int>(songId, plays);
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    if (song_root->insert(songId, plays) != 1)
    {
        delete node;
        return StatusType::FAILURE;
    }

    return StatusType::SUCCESS;
}

StatusType DSpotify::add_to_playlist(int playlistId, int songId)
{
    if (playlistId <= 0 || songId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    AVLNode<int> *search = song_root->find(songId);

    if (search != nullptr)
    {
        return StatusType::FAILURE;
    }

    AVLNode<Playlist<int>> *playlist = playlist_root->find(playlistId);
    if (playlist == nullptr)
    {
        return StatusType::FAILURE;
    }

    Playlist<int> *song_in_playlist_node = &playlist->data;

    if (song_in_playlist_node->get_songs_tree()->find(songId) != nullptr)
    {
        return StatusType::FAILURE;
    }

    int res = song_in_playlist_node->get_songs_tree()->insert(songId, 0);
    if (res != 1)
    {
        return StatusType::FAILURE;
    }
    else if (res == 1)
    {
        song_in_playlist_node->playlist_refers++;
        playlist->info++;
    }
    return StatusType::SUCCESS;
}

StatusType DSpotify::delete_song(int songId)
{
    if (songId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLNode<int> *search = song_root->find(songId);
    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }
    if (search->playlist_refers > 0)
    {
        return StatusType::FAILURE;
    }
    int res = song_root->remove(songId);
    if (res != 1)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}


StatusType DSpotify::remove_from_playlist(int playlistId, int songId)
{
    if (playlistId <= 0 || songId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLNode<Playlist<int>> *playlist = playlist_root->find(playlistId);
    if (playlist == nullptr)
    {
        return StatusType::FAILURE;
    }
    AVLNode<int> *search = song_root->find(songId);
    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }
    Playlist<int> *playlist_node = &playlist->data;
    int res = playlist_node->get_songs_tree()->remove(songId);
    if (res != 1)
    {
        return StatusType::FAILURE;
    }
    else if (res == 1)
    {
        playlist_node->playlist_refers--;
        playlist->info--;
    }

    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_plays(int songId)
{
    if (songId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLNode<int> *search = song_root->find(songId);
    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }
    return search->info;
    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_num_songs(int playlistId)
{
    if (playlistId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLNode<Playlist<int>> *playlist = playlist_root->find(playlistId);
    if (playlist == nullptr)
    {
        return StatusType::FAILURE;
    }
    return playlist->info;
    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_by_plays(int playlistId, int plays)
{
    if (playlistId <= 0 || plays < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLNode<Playlist<int>> *playlist = playlist_root->find(playlistId);
    if (playlist == nullptr)
    {
        return StatusType::FAILURE;
    }
    AVL<int> *song = playlist->data.get_songs_tree();
    AVLNode<int> *search = song->find_by_info_ceiling(song->get_root(), plays);
    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }
    return search->data;
    return StatusType::SUCCESS;
}

// אי אפשר להשתמש בווקטור רשימה תור וכו... לכן נעשה סוג של mergesort
//  על הפליליסטים: נכניס כל אחד לרשימה מקושרת בצורה ממוינת על ידי אינאורדר
// כמו המערך מצביעים בתרגול
//ואז נפעיל את אלגוריתם המיון בדומה למרג' סורט לפי 
//songid
//במהלכו עבור דופליקציות נשנה את המצביע שסופר רפרנסים (חהחהחה רפרנס)
//ככה שבכל כפילות נוריד 1 וגם בכל שיר שלא היה בראשון נוסיף 1 לגודל הפליליסט
//בסוף את המערך הממוין נכניס לעץ מאוזן נחילף מצביעים ונמחק את הישן
// סך הכל עוברים על 2 מערכים בגודל הפליליסט כמה פעמים 
//O(n) 
//בניית עץ חדש וגם מציאת השירים הכפולים 
//O(n)
// וחיפוש הפליליסטים
// O(log m )
//total = O(logm + n) ;)

StatusType DSpotify::unite_playlists(int playlistId1, int playlistId2)
{
    if (playlistId1 <= 0 || playlistId2 <= 0 || playlistId1 == playlistId2)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLNode<Playlist<int>> *playlist1 = playlist_root->find(playlistId1);
    AVLNode<Playlist<int>> *playlist2 = playlist_root->find(playlistId2);
    if (playlist1 == nullptr || playlist2 == nullptr)
    {
        return StatusType::FAILURE;
    }
    Playlist<int> &p1 = playlist1->data;
    Playlist<int> &p2 = playlist2->data;
    AVL<int> *playlist1_songs = p1.get_songs_tree();
    AVL<int> *playlist2_songs = p2.get_songs_tree();
    linkedListNode<int> *head_p1 = nullptr;
    linkedListNode<int> *tail_p1 = nullptr;
    linkedListNode<int> *head_p2 = nullptr;
    linkedListNode<int> *tail_p2 = nullptr;
    mergeAVLTools<int>::tree2list(playlist1_songs->get_root(), head_p1, tail_p1);
    mergeAVLTools<int>::tree2list(playlist2_songs->get_root(), head_p2, tail_p2);
    linkedListNode<int> *new_tree_head = nullptr;
    linkedListNode<int> *new_tree_tail = nullptr;

    int songs_from_p2 = 0;

    while (head_p1&&head_p2)
    {
        if (head_p1->data < head_p2->data)
        {
            linkedListNode<int>* node = new linkedListNode<int>(head_p1->data);
            mergeAVLTools<int>::add_to_list(new_tree_head,new_tree_tail,node);
            head_p1 = head_p1->next;
        }
        else if (head_p1->data > head_p2->data)
        {
            linkedListNode<int> *node = new linkedListNode<int>(head_p2->data);
            mergeAVLTools<int>::add_to_list(new_tree_head, new_tree_tail, node);
            head_p2 = head_p2->next;
            songs_from_p2++;
        }
        else // songp1 == song p2 => duplicate => sondi->playlistreff--
        {
            linkedListNode<int> *node = new linkedListNode<int>(head_p1->data);
            mergeAVLTools<int>::add_to_list(new_tree_head, new_tree_tail, node);
            AVLNode<int>* global_song = song_root->find(head_p1->data);
            if (global_song!= nullptr)
            {
                global_song->playlist_refers--;
            }
            head_p1 = head_p1->next;
            head_p2 = head_p2->next;
        }
    }

    while (head_p1)
    {
        linkedListNode<int> *node = new linkedListNode<int>(head_p1->data);
        mergeAVLTools<int>::add_to_list(new_tree_head, new_tree_tail, node);
        head_p1 = head_p1->next;
    }
    while (head_p2)
    {
        linkedListNode<int> *node = new linkedListNode<int>(head_p2->data);
        mergeAVLTools<int>::add_to_list(new_tree_head, new_tree_tail, node);
        head_p2 = head_p2->next;
        songs_from_p2++;
    }

    int new_tree_size = mergeAVLTools<int>::listSize(new_tree_head);
    AVLNode<int>* new_root = mergeAVLTools<int>::list2tree(new_tree_head,new_tree_size);

    delete playlist1_songs;
    AVL<int>* new_tree = new AVL<int>();
    new_tree->set_root(new_root);
    p1.set_songs_tree(new_tree);
    playlist1->info += songs_from_p2;
    playlist_root->remove(playlistId2);
    delete playlist2_songs;
    return StatusType::SUCCESS;
}
