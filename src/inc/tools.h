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
#include "main.h"




void initTool(void);

uint32_t  toolsGetUser();
void toolsGetToken(char* pToken);

std::vector<peeAlbum*>* 		toolsGetUserAlbums(uint32_t userId);
std::vector<peePlaylist*>* 		toolsGetUserPlaylists(uint32_t userId);
std::vector<peePodcast*>* 		toolsGetUserPodcasts(uint32_t userId); 	// Deezerpodcast
std::vector<peePodcast*>*  		toolsGetPodcast(void);				 	// podcast from config file
void 							toolsUpdateUserPodcastTracks(std::vector<peePodcastTrack*>* podcastList,peePodcast* pParent,char* htmlSource);

void toolsPrintAlbums(std::vector<peeAlbum*>* pAlbum);
void toolsPrintPlaylists(std::vector<peePlaylist*>* pPlaylist);
void toolsPrintPodcasts(std::vector<peePodcast*>* pPodcasts);
int toolsGetNext(stAppContext* pContext);
int toolsGetInput(stMouse* pMouse);
int toolsCleanUTF8(char* szString);


#endif /* INC_TOOLS_H_ */
