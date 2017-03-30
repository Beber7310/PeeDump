/*
 * guiPlaylist.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_Playlist_H_
#define GUI_Playlist_H_

#include "guiBase.h"
#include "peePlaylist.h"

using namespace std;

class guiPlaylist: public guiBase {
public:
	guiPlaylist();
	guiPlaylist(peePlaylist* pPlaylist);
	virtual ~guiPlaylist();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	peePlaylist* _pPlaylist;
	bool	_selected;
	int _color;
};

#endif /* GUI_guiPlaylist_H_ */
