/*
 * guiPodcast.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_guiPodcast_H_
#define GUI_guiPodcast_H_

#include "guiBase.h"
#include "peeAlbum.h"

using namespace std;

class guiPodcast: public guiBase {
public:
	guiPodcast();
	guiPodcast(peePodcast* pPodcast);
	virtual ~guiPodcast();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);
	void CreatePopup();

	peePodcast* _pPodcast;
	bool	_selected;
};

#endif /* GUI_guiPodcast_H_ */
