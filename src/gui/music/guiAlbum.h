/*
 * guiAlbum.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_GUIALBUM_H_
#define GUI_GUIALBUM_H_

#include "guiBase.h"
#include "peeAlbum.h"

using namespace std;

class guiAlbum: public guiBase {
public:
	guiAlbum();
	guiAlbum(peeAlbum* pAlbum);
	virtual ~guiAlbum();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	peeAlbum* 	_pAlbum;
	bool		_selected;
	int 		_color;
};

#endif /* GUI_GUIALBUM_H_ */
