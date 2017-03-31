/*
 * guiHeaterPopup.h
 *
 *  Created on: 21 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_FRAMEWORK_guiHeaterPopup_H_
#define GUI_FRAMEWORK_guiHeaterPopup_H_

#include <guiBase.h>
#include <guiButton.h>

class guiHeaterPopup: public guiBase {
public:
	guiHeaterPopup(int id);
	virtual ~guiHeaterPopup();

	void Render(void);
	void ComputeChildAbsPos(void);
	void Mouse(stMouse* pMouse);

	int _id;
	guiButton*		_heaterPlus;
	guiButton*		_heaterMoins;
	guiButton*		_heaterOk;
};

#endif /* GUI_FRAMEWORK_guiHeaterPopup_H_ */
