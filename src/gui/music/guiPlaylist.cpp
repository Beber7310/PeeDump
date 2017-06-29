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
	_color=232;
}

guiPlaylist::guiPlaylist(peePlaylist* pPlaylist) {
	_cy=100;
	_pPlaylist=pPlaylist;
	_selected=false;
	_color=232;
}

guiPlaylist::~guiPlaylist() {
	// TODO Auto-generated destructor stub
}

void guiPlaylist::Render(void)
{
	char tmp[32];
	_color++;
	if(_color>232)
		_color=232;

	ovgFill(44, 77, _color, 1);


	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);
	if(_pPlaylist)
	{
		RenderText(_x+5,_y+5,_pPlaylist->_name,25);

		sprintf(tmp,"(%i/%i)",_pPlaylist->_nbrTracksDownloaded,_pPlaylist->GetNbrTracks());
					RenderText(_x+450,_y+60,tmp,20);
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
			_color=100;
			_selected=true;

			_pPlaylist->fetchTracks();
			_pPlaylist->print();

			//deezerPostCommand(DEEZER_CMD_LOAD_PLAYLIST,_pPlaylist->_id,_pPlaylist->_name);
			deezerPostCommand(DEEZER_CMD_LOAD_PLAYLIST,_pPlaylist,NULL);
		}


	}

}
