/*
 * guiHeater.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_guiHeater_H_
#define GUI_guiHeater_H_

#include "guiBase.h"


using namespace std;

class guiHeater: public guiBase {
public:
	guiHeater();
	guiHeater(int id);
	virtual ~guiHeater();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	int _color;
	int _id;
};

#endif /* GUI_guiHeater_H_ */
