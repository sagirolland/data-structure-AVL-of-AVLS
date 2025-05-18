// You can edit anything you want in this file.
// However you need to implement all public DSpotify function, as provided below as a template

#include "dspotify25b1.h"
#include "AVL.h"

DSpotify::DSpotify()
{
    song_root = new AVL<int>();
    playlist_root = new AVL<Playlist<int>>();
}

DSpotify::~DSpotify(){
    delete song_root;
    delete playlist_root;
}

StatusType DSpotify::add_playlist(int playlistId){ // new avl root info = 1  
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
    StatusType res = playlist_root->insert(playlistId, 0);
    if (res != StatusType::SUCCESS)
    {
        delete node;
        return StatusType::FAILURE;
    }

    return res;
}

StatusType DSpotify::delete_playlist(int playlistId){
    if (playlistId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    AVLNode<Playlist<int>> *playlist = playlist_root->find(playlistId);
    if (&playlist == nullptr)
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
    if (song_root->find(songId) != nullptr)
    {
        return StatusType::FAILURE;
    }

    Song<int> *node = new Song<int>(songId, plays);
    if (node == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    if (song_root->insert(songId, plays) != StatusType::SUCCESS)
    {
        delete node;
        return StatusType::FAILURE;
    }

    return StatusType::SUCCESS;
}

StatusType DSpotify::add_to_playlist(int playlistId, int songId)
{ 
    if(playlistId <= 0 || songId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    AVLNode<int> *search = song_root->find(songId);

    if (search != nullptr)
    {
        return StatusType::FAILURE;
    }

    AVLNode<Playlist<int>>* playlist = playlist_root->find(playlistId);
    if (playlist == nullptr)
    {
        return StatusType::FAILURE;
    }

    Playlist<int> *playlist_node = &playlist->data;
    
    if (playlist_node->get_songs_tree()->find(songId) != nullptr)
    {
        return StatusType::FAILURE;
    }

    playlist->info++;
    search->index++;

    return playlist_node->get_songs_tree()->insert(songId, 0);
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
    AVLNode<Playlist<int>> *playlist = playlist_root->find(playlistId);
    if (&playlist == nullptr)
    {
        return StatusType::FAILURE;
    }
    AVLNode<int> *search = song_root->find(songId);
    if (song_root->remove(songId) == StatusType::FAILURE)
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
    AVLNode<Playlist<int>> *playlist = playlist_root->find(playlistId);
    return playlist->info;
    return StatusType::SUCCESS;
}

output_t<int> DSpotify::get_by_plays(int playlistId, int plays){
    AVLNode<Playlist<int>> *playlist = playlist_root->find(playlistId);
    if (&playlist == nullptr)
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
