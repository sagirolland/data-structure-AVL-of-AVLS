// You can edit anything you want in this file.
// However you need to implement all public DSpotify function, as provided below as a template

#include "dspotify25b1.h"
#include "AVL.h"

DSpotify::DSpotify()
{
    
}

DSpotify::~DSpotify(){

}

StatusType DSpotify::add_playlist(int playlistId){ // new avl root info = 1  
    
    return StatusType::FAILURE;
}

StatusType DSpotify::delete_playlist(int playlistId){
    return StatusType::FAILURE;
}

StatusType DSpotify::add_song(int songId, int plays){ // shared ptr to playlist = nullptr index = 0
    return StatusType::FAILURE;
}

StatusType DSpotify::add_to_playlist(int playlistId, int songId)
{ // insert new avlnode to playlist, playlist root info = +1 
    // song->shared_ptr = playlist song->index = +1
    // if song is already in playlist, return failure
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
    return StatusType::FAILURE;
}

output_t<int> DSpotify::get_plays(int songId){
    return 0;
}

output_t<int> DSpotify::get_num_songs(int playlistId){
    return 0;
}

output_t<int> DSpotify::get_by_plays(int playlistId, int plays){
    return 0;
}

StatusType DSpotify::unite_playlists(int playlistId1, int playlistId2){
    return StatusType::FAILURE;
}
