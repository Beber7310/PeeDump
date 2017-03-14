/*
 * GuiTabLayout.h
 *
 *  Created on: 12 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_GUITABLAYOUT_H_
#define GUI_GUITABLAYOUT_H_

#include "guiBase.h"

class guiTabLayout: public guiBase {
public:
	guiTabLayout();
	virtual ~guiTabLayout();
	void Render(void);
	void ComputeChildAbsPos(void);

	float				_xOrigin,_yOrigin; // keep record of the first visible pixel of the listview
	float				_xOriginTemp,_yOriginTemp; // keep record of the first visible pixel of the listview

};

#endif /* GUI_GUITABLAYOUT_H_ */
