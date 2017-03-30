/*
 * guiPlayer.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_guiPlayer_H_
#define GUI_guiPlayer_H_

#include "guiBase.h"

using namespace std;

class guiPlayer: public guiBase {
public:
	guiPlayer();
	guiPlayer(peePodcast* pPodcast);
	virtual ~guiPlayer();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	int colorNext;
	int colorPrev;
	int colorPause;

};

#endif /* GUI_guiPlayer_H_ */
