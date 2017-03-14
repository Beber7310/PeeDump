/*
 * guiList.h
 *
 *  Created on: 12 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_GUILIST_H_
#define GUI_GUILIST_H_

#include "guiBase.h"
#include <vector>

using namespace std;

class guiList: public guiBase {
public:
	guiList();
	virtual ~guiList();
	void Render(void);

	virtual void Mouse(stMouse* pMouse);
private:
	float				_xOrigin,_yOrigin; // keep record of the first visible pixel of the listview
	float				_xOriginTemp,_yOriginTemp; // keep record of the first visible pixel of the listview
	float 				_ySpacer,_xSpacer;


	void ComputeChildAbsPos(void);



};

#endif /* GUI_GUILIST_H_ */
