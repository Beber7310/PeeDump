/*
 * guiVmc.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_guiVmc_H_
#define GUI_guiVmc_H_

#include "guiBase.h"


using namespace std;

class guiVmc: public guiBase {
public:
	guiVmc();
	guiVmc(int id);
	virtual ~guiVmc();
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	int _color;
	int _id;
};

#endif /* GUI_guiVmc_H_ */
