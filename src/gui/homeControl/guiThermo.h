/*
 * guiThermo.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_guiThermo_H_
#define GUI_guiThermo_H_

#include "guiBase.h"


using namespace std;

class guiThermo: public guiBase {
public:
	guiThermo();
	guiThermo(int id);
	virtual ~guiThermo();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	int _color;
	int _id;
};

#endif /* GUI_guiThermo_H_ */
