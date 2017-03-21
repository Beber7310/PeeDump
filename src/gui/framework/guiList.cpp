/*
 * guiList.cpp
 *
 *  Created on: 12 mars 2017
 *      Author: Bertrand
 */

#include "guiList.h"

guiList::guiList() {
	// TODO Auto-generated constructor stub

	_xOrigin=0.0;
	_yOrigin=0.0;
	_xOriginTemp=0.0;
	_yOriginTemp=0.0;

	_xSpacer=5.0f;
	_ySpacer=5.0f;

}



guiList::~guiList() {
	// TODO Auto-generated destructor stub
	//FIXME add a free for each child
}





void guiList::Render(void)
{
	ComputeChildAbsPos();

	guiBase* pG=_ChildFirst;
	while(pG)
	{
		bool skip=false;
		if(pG->_y>_y+_cy)
			skip=true;
		if(pG->_y+pG->_cy<_y)
			skip=true;

		if(!skip)
			pG->Render();
		pG=pG->_BrotherNext;
	}

}

void guiList::ComputeChildAbsPos(void)
{
	float yCurrent = _y+_yOrigin+_yOriginTemp;
	guiBase* pG=_ChildFirst;
	while(pG)
	{
		pG->_y=yCurrent;
		pG->_cx=_cx-2*_xSpacer;
		pG->_x=_x+_xSpacer;

		yCurrent+=pG->_cy;
		yCurrent+=_ySpacer;

		pG=pG->_BrotherNext;
	}
}

void guiList::Mouse(stMouse* pMouse)
{
	guiBase::Mouse(pMouse);
	if(MouseIsInWindows(pMouse))
	{
		if(pMouse->t)
		{
			_yOriginTemp=pMouse->y-mouseOriginY;
		}
		else if(pMouse->LastTouch)
		{
			_yOrigin+=_yOriginTemp;
			_yOriginTemp=0;

			if(_yOrigin>0)
			{
				_yOrigin=0;
			}
		}

		guiBase* pG=_ChildFirst;
		while(pG)
		{
			pG->Mouse(pMouse);
			pG=pG->_BrotherNext;
		}


	}
}
