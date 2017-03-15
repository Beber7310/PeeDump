/*
 * guiPodcast.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <stddef.h>
#include "guiPodcast.h"
#include "deezer.h"

guiPodcast::guiPodcast() {
	// TODO Auto-generated constructor stub
	_cy=100;
	_pPodcast=NULL;
	_selected=false;
}

guiPodcast::guiPodcast(peePodcast* pPodcast) {
	_cy=100;
	_pPodcast=pPodcast;
	_selected=false;
}

guiPodcast::~guiPodcast() {
	// TODO Auto-generated destructor stub
}

void guiPodcast::Render(void)
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
	if(_pPodcast)
	{
		RenderText(_x+5,_y+5,_pPodcast->_title,20);

	}

}

void guiPodcast::Mouse(stMouse* pMouse)
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
			deezerPostCommand(DEEZER_CMD_LOAD_PODCAST,_pPodcast->_id);
		}


	}

}
