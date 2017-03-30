/*
 * guiPodcast.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <stddef.h>
#include "guiPodcast.h"
#include "guiPodcastTrack.h"
#include "guiList.h"
#include "guiRoot.h"
#include "guiVerticalSplit.h"
#include "guiExitPodcastTrack.h"
#include "deezer.h"

guiPodcast::guiPodcast() {
	// TODO Auto-generated constructor stub
	_cy=100;
	_pPodcast=NULL;
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
	ovgFill(44, 77, 232, 1);

	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);
	if(_pPodcast)
	{
		RenderText(_x+5,_y+5,_pPodcast->_titleUTF8,25);

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
			//deezerPostCommand(DEEZER_CMD_LOAD_PODCAST_MP3,_pPodcast->_traks->front()->_localPath);
			CreatePopup();
		}
	}
}

void guiPodcast::CreatePopup()
{
	guiVerticalSplit*		verticalSplit	= new guiVerticalSplit();
	guiList* 				tracks 			= new guiList();
	guiExitPodcastTrack*	exitPodcast		= new guiExitPodcastTrack();

	for (unsigned int ii = 0; ii < _pPodcast->GetNbrTracks(); ii++)
	{
		peePodcastTrack* res=_pPodcast->GetTracksAt(ii);
		if(res!=NULL)
			tracks->AddChild(new guiPodcastTrack(res));
	}

	verticalSplit->AddChild(tracks);
	verticalSplit->AddChild(exitPodcast);

	guiPopup(verticalSplit);
}
