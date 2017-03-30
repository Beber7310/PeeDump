/*
 * guiCourant.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_guiCourant_H_
#define GUI_guiCourant_H_

#include "guiBase.h"


using namespace std;

class guiCourant: public guiBase {
public:
	guiCourant();
	guiCourant(int id);
	virtual ~guiCourant();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	int _color;
};

#endif /* GUI_guiCourant_H_ */
