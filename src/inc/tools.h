/*
 * tools.h
 *
 *  Created on: 10 mars 2017
 *      Author: Bertrand
 */

#ifndef INC_TOOLS_H_
#define INC_TOOLS_H_
#include <vector>
#include "peeAlbum.h"
#include "peePlaylist.h"
#include "peePodcast.h"
#include "peeAlbum.h"
#include "peeTrack.h"
#include "main.h"


using namespace std;

void initTool(void);

uint32_t  toolsGetUser();
void cleanMP3String(char* str);
void toolsGetToken(char* pToken);

std::vector<peeAlbum*>* 		toolsGetUserAlbums(uint32_t userId);
std::vector<peeAlbum*>* 		toolsGetUserAlbumsIndex(uint32_t userId,int index,vector<peeAlbum*>* retAlbum);
std::vector<peeTrack*>* 		toolsGetUserAlbumTracks(peeAlbum* pAlbum);

std::vector<peePlaylist*>* 		toolsGetUserPlaylists(uint32_t userId);
std::vector<peeTrack*>* 		toolsGetUserPlaylistTracks(peePlaylist* pPlaylist);

std::vector<peePodcast*>*  		toolsGetPodcast(void);				 	// podcast from config file
void 							toolsUpdateUserPodcastTracks(std::vector<peePodcastTrack*>* podcastList,peePodcast* pParent,char* htmlSource);

void toolsPrintAlbums(std::vector<peeAlbum*>* pAlbum);
void toolsPrintPlaylists(std::vector<peePlaylist*>* pPlaylist);
void toolsPrintPodcasts(std::vector<peePodcast*>* pPodcasts);
int toolsGetNext(stAppContext* pContext);
int toolsGetInput(stMouse* pMouse);
int toolsCleanUTF8(char* szString);
void toolsDownloaderTracks(vector<peeAlbum*>* pAlbum);
void toolsDownloaderPlaylist(vector<peePlaylist*>* pPlaylist);

#endif /* INC_TOOLS_H_ */
