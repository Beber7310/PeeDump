/*
 * guiVerticalSplit.h
 *
 *  Created on: 21 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_FRAMEWORK_GUIVERTICALSPLIT_H_
#define GUI_FRAMEWORK_GUIVERTICALSPLIT_H_

#include <guiBase.h>

class guiVerticalSplit: public guiBase {
public:
	guiVerticalSplit();
	virtual ~guiVerticalSplit();

	void Render(void);
	void ComputeChildAbsPos(void);
	void Mouse(stMouse* pMouse);

	int _verticalSplitSize;
};

#endif /* GUI_FRAMEWORK_GUIVERTICALSPLIT_H_ */
