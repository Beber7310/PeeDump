/*
 * guiMP3.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_MP3_H_
#define GUI_MP3_H_

#include "guiBase.h"
#include "peePlaylist.h"

using namespace std;

class guiMP3: public guiBase {
public:
	guiMP3();
	guiMP3(char* dir);
	virtual ~guiMP3();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	char* _dir;
	bool	_selected;
	int _color;
};

#endif /* GUI_MP3_H_ */
