/*
 * guiPlaylist.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <stddef.h>
#include "guiPlaylist.h"
#include "deezer.h"

guiPlaylist::guiPlaylist() {
	// TODO Auto-generated constructor stub
	_cy=200;
	_pPlaylist=NULL;
	_selected=false;
}

guiPlaylist::guiPlaylist(peePlaylist* pPlaylist) {
	_cy=100;
	_pPlaylist=pPlaylist;
	_selected=false;
}

guiPlaylist::~guiPlaylist() {
	// TODO Auto-generated destructor stub
}

void guiPlaylist::Render(void)
{
	if(_selected){
		ovgFill(44, 77, 232, 1);
	}
	else
	{
		ovgFill(232, 77, 44, 1);
	}

	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);
	if(_pPlaylist)
	{
		RenderText(_x+5,_y+5,_pPlaylist->_name,20);
	}

}

void guiPlaylist::Mouse(stMouse* pMouse)
{
	guiBase::Mouse(pMouse);
	if(MouseIsInWindows(pMouse))
	{
		if(pMouse->t)
		{

		}
		else if(pMouse->Click)
		{

			_selected=true;
			deezerPostCommand(DEEZER_CMD_LOAD_PLAYLIST,_pPlaylist->_id);
		}


	}

}
