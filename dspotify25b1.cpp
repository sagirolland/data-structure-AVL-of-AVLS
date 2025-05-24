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

        return StatusType::INVALID_INPUT;
    }
    if (playlist_root->findwithint(playlistId) != nullptr)
    {

        return StatusType::FAILURE;
    }
    Playlist *playlist = new Playlist();
    int res = playlist_root->insert(playlist, playlistId);
    if (res != 1)
    {
        return StatusType::FAILURE;
    }

    return StatusType::SUCCESS;
}

StatusType DSpotify::delete_playlist(int playlistId)
{
    if (playlistId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    
    if (playlist_root->findwithint(playlistId) == nullptr)
    {
        return StatusType::FAILURE;
    }
    if(playlist_root->findwithint(playlistId)->data->size > 0)
    {
        return StatusType::FAILURE;
    }
    playlist_root->remove(playlistId);
    return StatusType::SUCCESS;
}

StatusType DSpotify::add_song(int songId, int plays)
{
    if (songId <= 0 || plays < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (song_root->findwithint(songId) != nullptr)
    {
        return StatusType::FAILURE;
    }

    Song *song = new Song( plays);
    if (song_root->insert(song, songId) != 1)
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
  
    AVLNode<Song*> *song_node = song_root->findwithint(songId);
    if (song_node == nullptr)
    {
        return StatusType::FAILURE; // Song must exist globally
    }
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

    Song*  song = song_node->data;
    AVL<Song*> *song_root = playlist_data->get_songs_tree();
    int res =song_root->insert(song, songId);
    if (res != 1)
    {
        return StatusType::FAILURE;
    }

    song_node->data->playlist_refers++;
    playlist_data->size++;
    return StatusType::SUCCESS;
}

StatusType DSpotify::delete_song(int songId)
{
    if (songId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLNode<Song*> *search = song_root->findwithint(songId);

    if (search == nullptr)
    {

        return StatusType::FAILURE;
    }

    if (search->data->playlist_refers > 0)
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
    AVLNode<Playlist *> *playlist = playlist_root->findwithint(playlistId);
    if (playlist == nullptr)
    {
        return StatusType::FAILURE;
    }
    AVLNode<Song *> *search = song_root->findwithint(songId);

    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }


    Playlist *playlist_node = playlist->data;
    AVL<Song *> *playlist_tree = playlist_node->get_songs_tree();
    AVLNode<Song *> *song_in_playlist = playlist_tree->findwithint(songId);
    if (song_in_playlist == nullptr || song_in_playlist->data != search->data)
    {
        return StatusType::FAILURE;
    }

    int res = playlist_tree->remove(songId);
    if (res != 1)
    {
        return StatusType::FAILURE;
    }
    // Only update if removal succeeded
    search->data->playlist_refers--;
    playlist_node->size--;

    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_plays(int songId)
{
    if (songId <= 0)
    {
        
        return StatusType::INVALID_INPUT;
    }
    AVLNode<Song *> *search = song_root->findwithint(songId);

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

        return StatusType::INVALID_INPUT;
    }
    AVLNode<Playlist *> *playlist = playlist_root->findwithint(playlistId);
    if (playlist == nullptr)
    {

        return StatusType::FAILURE;
    }

    return playlist->data->size;
    return StatusType::SUCCESS;
}

// output_t<int> DSpotify::get_by_plays(int playlistId, int plays)
// {
//     if (playlistId <= 0 || plays < 0)
//     {
//         return StatusType::INVALID_INPUT;
//     }
//     AVLNode<Playlist *> *playlist = playlist_root->findwithint(playlistId);
//     if (playlist == nullptr)
//     {
//         return StatusType::FAILURE;
//     }

//     AVL<Song*> *song = playlist->data->playlist_song_tree_root;
//     AVLNode<Song*> *search = song->find_by_info_ceiling(song->get_root(), plays);

//     if (search == nullptr)
//     {
//         return StatusType::FAILURE;
//     }
//     return search->uid;
//     return StatusType::SUCCESS;
// }
output_t<int> DSpotify::get_by_plays(int playlistId, int plays)
{
    if (playlistId <= 0 || plays < 0)
        return StatusType::INVALID_INPUT;

    AVLNode<Playlist *> *playlist = playlist_root->findwithint(playlistId);
    if (playlist == nullptr)
        return StatusType::FAILURE;

    AVL<Song *> *song_tree = playlist->data->get_songs_tree();
    if (!song_tree)
        return StatusType::FAILURE;

    AVLNode<Song *> *root = song_tree->get_root();
    if (!root)
        return StatusType::FAILURE;

    PlaysSearchContext context;
    context.plays = plays;
    context.best_id = -1;
    context.best_plays = -1;
    context.found = false;

    AVL<Song *>::inorder_with_context(root, find_by_plays_cb, &context);

    if (!context.found)
        return StatusType::FAILURE;
    return context.best_id;
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
    // Step 1: Validate input
    if (playlistId1 <= 0 || playlistId2 <= 0 || playlistId1 == playlistId2)
    {
        return StatusType::INVALID_INPUT;
    }

    // Step 2: Find both playlists
    AVLNode<Playlist *> *node1 = playlist_root->findwithint(playlistId1);
    AVLNode<Playlist *> *node2 = playlist_root->findwithint(playlistId2);

    if (!node1 || !node2)
    {
        return StatusType::FAILURE;
    }

    Playlist *p1 = node1->data;
    Playlist *p2 = node2->data;

    // Step 3: Get roots of both AVL trees
    AVL<Song *> *tree1 = p1->get_songs_tree();
    AVL<Song *> *tree2 = p2->get_songs_tree();

    AVLNode<Song *> *root1 = tree1 ? tree1->get_root() : nullptr;
    AVLNode<Song *> *root2 = tree2 ? tree2->get_root() : nullptr;

    // Step 4: Merge the two AVL trees using the new tool
    AVLNode<Song *> *new_root = mergeAVLTools<Song *>::mergeTrees(root1, root2);

    // Step 5: Set new root to new AVL tree
    AVL<Song *> *merged_tree = new AVL<Song *>();
    merged_tree->set_root(new_root);

    // Step 6: Assign to playlist1 and clean up playlist2
    p1->set_songs_tree(merged_tree);
    int merged_size = mergeAVLTools<Song *>::count_nodes(merged_tree->get_root());
    p1->size = merged_size;
    p2->set_songs_tree(nullptr);

    playlist_root->remove(playlistId2);
    delete p2;

    return StatusType::SUCCESS;
}
