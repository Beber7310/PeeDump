/*
 * guiPodcastTrack.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <stddef.h>
#include <time.h>
#include "guiPodcastTrack.h"
#include "guiRoot.h"
#include "deezer.h"

guiPodcastTrack::guiPodcastTrack() {
	// TODO Auto-generated constructor stub
	_cy=100;
	_pTrack=NULL;
	_selected=false;
	_color=232;
}

guiPodcastTrack::guiPodcastTrack(peePodcastTrack* pTrack) {
	_cy=100;
	_pTrack=pTrack;
	_selected=false;
	_color=232;
}

guiPodcastTrack::~guiPodcastTrack() {
	// TODO Auto-generated destructor stub
}

void guiPodcastTrack::Render(void)
{
	_color++;
	if(_color>232)
		_color=232;

	if(_pTrack->_downloaded){
		ovgFill(44, 77, _color, 1);
	}
	else
	{
		ovgFill(232, 77, 44, 1);
	}

	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);

	if(_pTrack->_downloaded){
		ovgFill(255, 255, 255, 1);
	}
	else
	{
		ovgFill(128, 128, 128, 1);
	}

	if(_pTrack)
	{
		char buffer [80];

		strftime (buffer,80,"%d/%m/%Y",&_pTrack->_date);

		RenderText(_x+5,_y+5,_pTrack->_title,20);
		RenderText(_x+350,_y+60,buffer,15);
	}

}

void guiPodcastTrack::Mouse(stMouse* pMouse)
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
			deezerPostCommand(DEEZER_CMD_LOAD_PODCAST_MP3,_pTrack->_localPath,_pTrack->_title);
			//guiExitPopup();
		}


	}

}
