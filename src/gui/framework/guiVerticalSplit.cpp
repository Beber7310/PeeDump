/*
 * guiVerticalSplit.cpp
 *
 *  Created on: 21 mars 2017
 *      Author: Bertrand
 */

#include <guiVerticalSplit.h>

guiVerticalSplit::guiVerticalSplit() {
	// TODO Auto-generated constructor stub
	_verticalSplitSize=900;
}

guiVerticalSplit::~guiVerticalSplit() {
	// TODO Auto-generated destructor stub

}

void guiVerticalSplit::Render(void)
{
	ComputeChildAbsPos();

	guiBase* pG=_ChildFirst;
	while(pG)
	{
		bool skip=false;

		if(pG->_x>=_cx)
			skip=true;
		if((pG->_x+pG->_cx)<=_x)
			skip=true;
		if(!skip)
			pG->Render();


		pG=pG->_BrotherNext;
	}


}

void guiVerticalSplit::ComputeChildAbsPos(void)
{
	float xCurrent = _x;
	float yCurrent = _y;

	guiBase* pFirst;
	guiBase* pSecond;

	pFirst=_ChildFirst;

	if(!_ChildFirst)
		return;

	pSecond=_ChildFirst->_BrotherNext;

	if(!pSecond)
		return;

	pFirst->_x=xCurrent;
	pFirst->_y=yCurrent;

	pFirst->_cx=_cx;
	pFirst->_cy=_verticalSplitSize;

	pSecond->_x=xCurrent;
	pSecond->_y=_verticalSplitSize;

	pSecond->_cx=_cx;
	pSecond->_cy=_cy-_verticalSplitSize;


}

void guiVerticalSplit::Mouse(stMouse* pMouse)
{

	guiBase::Mouse(pMouse);
	if(MouseIsInWindows(pMouse))
	{

		guiBase* pG=_ChildFirst;
		while(pG)
		{
			pG->Mouse(pMouse);
			pG=pG->_BrotherNext;
		}
	}
}
