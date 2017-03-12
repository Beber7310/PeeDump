/*
 * tools.h
 *
 *  Created on: 10 mars 2017
 *      Author: Bertrand
 */

#ifndef INC_TOOLS_H_
#define INC_TOOLS_H_
#include <list>
#include "peeAlbum.h"
#include "peePlaylist.h"


uint32_t  toolsGetUser();
void toolsGetToken(char* pToken);

std::list<peeAlbum*>* toolsGetUserAlbums(uint32_t userId);
std::list<peePlaylist*>* toolsGetUserPlaylists(uint32_t userId);
void toolsPrintAlbums(std::list<peeAlbum*>* pAlbum);
void toolsPrintPlaylists(std::list<peePlaylist*>* pPlaylist);
int toolsGetNext();

#endif /* INC_TOOLS_H_ */
