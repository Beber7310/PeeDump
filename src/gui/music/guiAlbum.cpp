/*
 * guiAlbum.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <stddef.h>
#include "guiAlbum.h"
#include "deezer.h"

guiAlbum::guiAlbum() {
	// TODO Auto-generated constructor stub
	_cy=100;
	_pAlbum=NULL;
	_selected=false;
	_color=232;
}

guiAlbum::guiAlbum(peeAlbum* pAlbum) {
	_cy=100;
	_pAlbum=pAlbum;
	_selected=false;
	_color=232;
}

guiAlbum::~guiAlbum() {
	// TODO Auto-generated destructor stub
}

void guiAlbum::Render(void)
{
	_color++;
		if(_color>232)
			_color=232;
	ovgFill(44, 77, _color, 1);


	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);
	if(_pAlbum)
	{
		RenderText(_x+5,_y+5,_pAlbum->_artisteName,25);
		RenderText(_x+40,_y+50,_pAlbum->_albumName,20);
	}

}

void guiAlbum::Mouse(stMouse* pMouse)
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
			_pAlbum->fetchTracks();
			_pAlbum->print();

			//deezerPostCommand(DEEZER_CMD_LOAD_ALBUM,_pAlbum->_id,_pAlbum->_albumName);
			deezerPostCommand(DEEZER_CMD_LOAD_ALBUM,_pAlbum,NULL);

		}
	}

}
