/*
 * guiExitPodcastTrack.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <stddef.h>
#include "guiExitPodcastTrack.h"
#include "deezer.h"
#include "guiRoot.h"

guiExitPodcastTrack::guiExitPodcastTrack() {
	// TODO Auto-generated constructor stub
	_cy=100;
}

guiExitPodcastTrack::guiExitPodcastTrack(peeAlbum* pAlbum) {
	_cy=100;
}

guiExitPodcastTrack::~guiExitPodcastTrack() {
	// TODO Auto-generated destructor stub
}

void guiExitPodcastTrack::Render(void)
{

	ovgFill(232, 77, 44, 1);
	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);
	RenderText(_x+250,_y+20,"Back",30);

}

void guiExitPodcastTrack::Mouse(stMouse* pMouse)
{
	guiBase::Mouse(pMouse);
	if(MouseIsInWindows(pMouse))
	{
		if(pMouse->t)
		{

		}
		else if(pMouse->Click)
		{
			guiExitPopup();
		}
	}
}
