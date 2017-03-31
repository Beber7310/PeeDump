/*
 * guiVmcPopup.h
 *
 *  Created on: 21 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_FRAMEWORK_guiVmcPopup_H_
#define GUI_FRAMEWORK_guiVmcPopup_H_

#include <guiBase.h>
#include <guiButton.h>

class guiLightPopup: public guiBase {
public:
	guiLightPopup(int id);
	virtual ~guiLightPopup();

	void Render(void);
	void ComputeChildAbsPos(void);
	void Mouse(stMouse* pMouse);

	int _id;
	guiButton*		_heaterPlus;
	guiButton*		_heaterMoins;
	guiButton*		_heaterOk;
};

#endif /* GUI_FRAMEWORK_guiVmcPopup_H_ */
