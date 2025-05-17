// You can edit anything you want in this file.
// However you need to implement all public DSpotify function, as provided below as a template

#include "dspotify25b1.h"
#include "AVL.h"

DSpotify::DSpotify(): root(nullptr), song_root(nullptr)
{
    
}

DSpotify::~DSpotify(){
}

StatusType DSpotify::add_playlist(int playlistId){ // new avl root info = 1  
    if (playlistId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (root == nullptr)
    {
        root = new Playlist<int>(playlistId);
        return StatusType::SUCCESS;
    }

    AVLNode<int> *search = find(playlistId);
    
    if (search != nullptr)
    {
        return StatusType::FAILURE;
    }

    Playlist<int> *node = new Playlist<int>(playlistId);

    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    if (insert(playlistId, 1) == StatusType::FAILURE)
    {
        delete node;
        return StatusType::FAILURE;
    }

    return StatusType::SUCCESS;
}

StatusType DSpotify::delete_playlist(int playlistId){
    AVLNode<int> *node = find(playlistId);
    if (node == nullptr)
    {
        return StatusType::FAILURE;
    }
    if (remove(playlistId) == StatusType::FAILURE)
    {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType DSpotify::add_song(int songId, int plays){ // shared ptr to playlist = nullptr index = 0
    if (songId <= 0 || plays < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (song_root == nullptr)
    {
        song_root = new Song<int>(songId, plays);
        return StatusType::SUCCESS;
    }
    AVLNode<int> *search = song_root->find(songId);
    if (search != nullptr)
    {
        return StatusType::FAILURE;
    }
    Song<int> *node = new Song<int>(songId, plays);
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    if (song_root->insert(songId, plays) == StatusType::FAILURE)
    {
        delete node;
        return StatusType::FAILURE;
    }

    return StatusType::SUCCESS;
}

StatusType DSpotify::add_to_playlist(int playlistId, int songId)
{ // insert new avlnode to playlist, playlist root info = +1 
    // song->shared_ptr = playlist song->index = +1
    // if song is already in playlist, return failure
    AVLNode<int> *search = find(playlistId);

    if (search != nullptr)
    {
        return StatusType::FAILURE;
    }

    AVLNode<int> *search = song_root->find(songId);
    if (search != nullptr)
    {
        return StatusType::FAILURE;
    }

    AVLNode<int> *newroot = search;
    Song<int> *node = new Song<int>(songId, 0);
    insert(songId, 0);

    return StatusType::FAILURE;
}

StatusType DSpotify::delete_song(int songId){
    // find song in song avl tree
    // if song is in a playlist, return failure:
    // if song->shared_ptr != nullptr, and song->index != 0 return failure
    // else delete song from avl tree
    return StatusType::FAILURE;
}

StatusType DSpotify::remove_from_playlist(int playlistId, int songId){
    // if sucssesfull delete avlnode from playlist, playlist root info = -1
    AVLNode<int> *search = find(playlistId);
    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }
    AVLNode<int> *search = song_root->find(songId);
    if (remove(songId) == StatusType::FAILURE)
    {
        return StatusType::FAILURE;
    }
    search->info--;
    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_plays(int songId){
    AVLNode<int> *search = song_root->find(songId);
    return search->info;
    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_num_songs(int playlistId){
    AVLNode<int> *search = find(playlistId);
    return search->info;
    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_by_plays(int playlistId, int plays){
    AVLNode<int> *search = find(playlistId);
    if (search == nullptr)
    {
        return StatusType::FAILURE;
    }
    AVLNode<int> *search = song_root->find(plays);
    return search->info;
    return StatusType::SUCCESS;
}

StatusType DSpotify::unite_playlists(int playlistId1, int playlistId2){
    return StatusType::FAILURE;
}
