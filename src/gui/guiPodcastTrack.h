/*
 * guiPodcastTrack.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_guiPodcastTrack_H_
#define GUI_guiPodcastTrack_H_

#include "guiBase.h"


using namespace std;

class guiPodcastTrack: public guiBase {
public:
	guiPodcastTrack();
	guiPodcastTrack(peePodcastTrack* pTrack);
	virtual ~guiPodcastTrack();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	peePodcastTrack* _pTrack;
	bool	_selected;
};

#endif /* GUI_guiPodcastTrack_H_ */
