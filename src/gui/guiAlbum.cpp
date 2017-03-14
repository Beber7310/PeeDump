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
	_cy=200;
	_pAlbum=NULL;
	_selected=false;
}

guiAlbum::guiAlbum(peeAlbum* pAlbum) {
	_cy=100;
	_pAlbum=pAlbum;
	_selected=false;
}

guiAlbum::~guiAlbum() {
	// TODO Auto-generated destructor stub
}

void guiAlbum::Render(void)
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
	if(_pAlbum)
	{
		RenderText(_x+5,_y+5,_pAlbum->_albumName,20);
		RenderText(_x+5,_y+30,_pAlbum->_artisteName,15);
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

			_selected=true;
			deezerPostCommand(DEEZER_CMD_LOAD_ALBUM,_pAlbum->_id);
		}


	}

}
