/*
 * guiExitPodcastTrack.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_guiExitPodcastTrack_H_
#define GUI_guiExitPodcastTrack_H_

#include "guiBase.h"
#include "peeAlbum.h"

using namespace std;

class guiExitPodcastTrack: public guiBase {
public:
	guiExitPodcastTrack();
	guiExitPodcastTrack(peeAlbum* pAlbum);
	virtual ~guiExitPodcastTrack();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);


};

#endif /* GUI_guiExitPodcastTrack_H_ */
