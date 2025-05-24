// You can edit anything you want in this file.
// However you need to implement all public DSpotify function, as provided below as a template

#include "dspotify25b1.h"
#include "AVL.h"

DSpotify::DSpotify()
{
    song_root = new AVL<Song *>();
    playlist_root = new AVL<Playlist *>();
    playlist_root->set_root(nullptr);
    song_root->set_root(nullptr);
}

DSpotify::~DSpotify()
{
    delete song_root;
    delete playlist_root;
}

StatusType DSpotify::add_playlist(int playlistId)
{
    if (playlistId <= 0)
    {
        std ::cout << "Error in add playlist 1" << std::endl;

        return StatusType::INVALID_INPUT;
    }
    Playlist playlist_key(playlistId);
    if (playlist_root->find(&playlist_key) != nullptr)
    {
        std ::cout << "Error in add playlist 2 " << std::endl;

        return StatusType::FAILURE;
    }
    Playlist *playlist = new Playlist(playlistId);
    int res = playlist_root->insert(playlist, 0);
    if (res != 1)
    {
        std ::cout << "Error in add playlist 3 " << std::endl;
        return StatusType::FAILURE;
    }
    std ::cout << "success in add playlist" << std::endl;

    return StatusType::SUCCESS;
}

StatusType DSpotify::delete_playlist(int playlistId)
{
    if (playlistId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    Playlist playlist_key(playlistId);
    if (playlist_root->find(&playlist_key) == nullptr)
    {
        return StatusType::FAILURE;
    }
    playlist_root->remove(&playlist_key);
    return StatusType::SUCCESS;
}

StatusType DSpotify::add_song(int songId, int plays)
{
    if (songId <= 0 || plays < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    Song song_key(songId, 0);
    if (song_root->find(&song_key) != nullptr)
    {
        return StatusType::FAILURE;
    }

    Song *song = new Song(songId, plays);
    if (song_root->insert(song, plays) != 1)
    {
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

    Song song_key(songId, 0);
    AVLNode<Song *> *song_node = song_root->findwithint(songId);
    if (song_node == nullptr)
    {
        return StatusType::FAILURE; // Song must exist globally
    }
    Playlist playlist_key(playlistId);
    AVLNode<Playlist *> *playlist_node = playlist_root->findwithint(playlistId);
    if (playlist_node == nullptr)
    {
        return StatusType::FAILURE;
    }

    Playlist *playlist_data = playlist_node->data;

    if (playlist_data->get_songs_tree()->findwithint(songId) != nullptr)
    {
        return StatusType::FAILURE;
    }

    Song *song = new Song(songId, 0);
    AVL<Song *> *song_root = playlist_data->get_songs_tree();
    int res = song_root->insert(song, 0);
    if (res != 1)
    {
        return StatusType::FAILURE;
    }

    song_node->data->playlist_refers++;
    playlist_data->info++;

    return StatusType::SUCCESS;
}

StatusType DSpotify::delete_song(int songId)
{
    if (songId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    Song song_key(songId, 0);
    AVLNode<Song *> *search = song_root->find(&song_key);

    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }
    if (search->data->playlist_refers > 0)
    {
        return StatusType::FAILURE;
    }
    int res = song_root->remove(&song_key);
    if (res != 1)
    {
        return StatusType::FAILURE;
    }
    // Clean up memory to avoid leaks
    delete search->data;
    return StatusType::SUCCESS;
}

StatusType DSpotify::remove_from_playlist(int playlistId, int songId)
{
    if (playlistId <= 0 || songId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    Playlist playlist_key(playlistId);
    AVLNode<Playlist *> *playlist = playlist_root->find(&playlist_key);
    if (playlist == nullptr)
    {
        return StatusType::FAILURE;
    }
    Song song_key(songId, 0);
    AVLNode<Song *> *search = song_root->find(&song_key);

    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }

    Song *song = new Song(songId, 0);

    Playlist *playlist_node = playlist->data;
    int res = playlist_node->get_songs_tree()->remove(song);
    if (res != 1)
    {
        return StatusType::FAILURE;
    }
    // Only update if removal succeeded
    search->data->playlist_refers--;
    playlist_node->info--;

    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_plays(int songId)
{
    if (songId <= 0)
    {

        return StatusType::INVALID_INPUT;
    }
    Song song_key(songId, 0);
    AVLNode<Song *> *search = song_root->find(&song_key);

    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }
    return search->data->plays;
    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_num_songs(int playlistId)
{
    if (playlistId <= 0)
    {
        std ::cout << "Error in get num songs in playlist1" << std::endl;

        return StatusType::INVALID_INPUT;
    }
    Playlist playlist_key(playlistId);
    AVLNode<Playlist *> *playlist = playlist_root->findwithint(playlistId);
    if (playlist == nullptr)
    {
        std ::cout << "Error in get num songs in playlist2" << std::endl;

        return StatusType::FAILURE;
    }
    std ::cout << "success in get num songs in playlist1" << std::endl;

    return playlist->data->info;
    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_by_plays(int playlistId, int plays)
{
    // if (playlistId <= 0 || plays < 0)
    // {
    //     return StatusType::INVALID_INPUT;
    // }
    // Playlist playlist_key(playlistId);
    // AVLNode<Playlist *> *playlist = playlist_root->find(&playlist_key);
    // if (playlist == nullptr)
    // {
    //     return StatusType::FAILURE;
    // }

    // AVL<int> *song = playlist->data->get_songs_tree();
    // AVLNode<int> *search = song->find_by_info_ceiling(song->get_root(), plays);

    // if (search == nullptr)
    // {
    //     return StatusType::FAILURE;
    // }
    // return search->data;
    return StatusType::SUCCESS;
}

// אי אפשר להשתמש בווקטור רשימה תור וכו... לכן נעשה סוג של mergesort
//  על הפליליסטים: נכניס כל אחד לרשימה מקושרת בצורה ממוינת על ידי אינאורדר
// כמו המערך מצביעים בתרגול
// ואז נפעיל את אלגוריתם המיון בדומה למרג' סורט לפי
// songid
// במהלכו עבור דופליקציות נשנה את המצביע שסופר רפרנסים (חהחהחה רפרנס)
// ככה שבכל כפילות נוריד 1 וגם בכל שיר שלא היה בראשון נוסיף 1 לגודל הפליליסט
// בסוף את המערך הממוין נכניס לעץ מאוזן נחילף מצביעים ונמחק את הישן
// סך הכל עוברים על 2 מערכים בגודל הפליליסט כמה פעמים
// O(n)
// בניית עץ חדש וגם מציאת השירים הכפולים
// O(n)
// וחיפוש הפליליסטים
// O(log m )
// total = O(logm + n) ;)
StatusType DSpotify::unite_playlists(int playlistId1, int playlistId2)
{
    //     if (playlistId1 <= 0 || playlistId2 <= 0 || playlistId1 == playlistId2)
    //     {
    //         return StatusType::INVALID_INPUT;
    //     }
    //     Playlist playlist_key1(playlistId1);
    //     Playlist playlist_key2(playlistId2);
    //     AVLNode<Playlist *> *playlist1 = playlist_root->find(&playlist_key1);
    //     AVLNode<Playlist *> *playlist2 = playlist_root->find(&playlist_key2);

    //     if (playlist1 == nullptr || playlist2 == nullptr)
    //     {
    //         return StatusType::FAILURE;
    //     }
    //     Playlist *p1 = playlist1->data;
    //     Playlist *p2 = playlist2->data;
    //     AVL<int> *playlist1_songs = p1->get_songs_tree();
    //     AVL<int> *playlist2_songs = p2->get_songs_tree();
    //     linkedListNode<int> *head_p1 = nullptr;
    //     linkedListNode<int> *tail_p1 = nullptr;
    //     linkedListNode<int> *head_p2 = nullptr;
    //     linkedListNode<int> *tail_p2 = nullptr;
    //     mergeAVLTools<int>::tree2list(playlist1_songs->get_root(), head_p1, tail_p1);
    //     mergeAVLTools<int>::tree2list(playlist2_songs->get_root(), head_p2, tail_p2);
    //     linkedListNode<int> *new_tree_head = nullptr;
    //     linkedListNode<int> *new_tree_tail = nullptr;

    //     int songs_from_p2 = 0;

    //     while (head_p1 && head_p2)
    //     {
    //         if (head_p1->data < head_p2->data)
    //         {
    //             linkedListNode<int> *node = new linkedListNode<int>(head_p1->data);
    //             mergeAVLTools<int>::add_to_list(new_tree_head, new_tree_tail, node);
    //             head_p1 = head_p1->next;
    //         }
    //         else if (head_p1->data > head_p2->data)
    //         {
    //             linkedListNode<int> *node = new linkedListNode<int>(head_p2->data);
    //             mergeAVLTools<int>::add_to_list(new_tree_head, new_tree_tail, node);
    //             head_p2 = head_p2->next;
    //             songs_from_p2++;
    //         }
    //         else // songp1 == song p2 => duplicate => sondi->playlistreff--
    //         {
    //             linkedListNode<int> *node = new linkedListNode<int>(head_p1->data);
    //             mergeAVLTools<int>::add_to_list(new_tree_head, new_tree_tail, node);
    //             Song song_key(head_p1->data, 0);
    //             AVLNode<Song *> *global_song = song_root->find(&song_key);
    //             if (global_song != nullptr)
    //             {
    //                 global_song->data->playlist_refers--;
    //             }
    //             head_p1 = head_p1->next;
    //             head_p2 = head_p2->next;
    //         }
    //     }

    //     while (head_p1)
    //     {
    //         linkedListNode<int> *node = new linkedListNode<int>(head_p1->data);
    //         mergeAVLTools<int>::add_to_list(new_tree_head, new_tree_tail, node);
    //         head_p1 = head_p1->next;
    //     }
    //     while (head_p2)
    //     {
    //         linkedListNode<int> *node = new linkedListNode<int>(head_p2->data);
    //         mergeAVLTools<int>::add_to_list(new_tree_head, new_tree_tail, node);
    //         head_p2 = head_p2->next;
    //         songs_from_p2++;
    //     }

    //     int new_tree_size = mergeAVLTools<int>::listSize(new_tree_head);
    //     AVLNode<int> *new_root = mergeAVLTools<int>::list2tree(new_tree_head, new_tree_size);

    //     AVL<int> *new_tree = new AVL<int>();
    //     new_tree->set_root(new_root);
    //     p1->set_songs_tree(new_tree);
    //     p1->info += songs_from_p2;
    //     p2->set_songs_tree(nullptr);
    //     playlist_root->remove(&playlist_key2);
    //     delete p2; // Clean up memory for removed playlist

    //     linkedListNode<int> *tmp = new_tree_head;
    //     while (tmp)
    //     {
    //         linkedListNode<int> *next = tmp->next;
    //         delete tmp;
    //         tmp = next;
    //     }
    return StatusType::SUCCESS;
}