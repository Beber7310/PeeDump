/*
 * GuiTabLayout.cpp
 *
 *  Created on: 12 mars 2017
 *      Author: Bertrand
 */

#include "guiTabLayout.h"

guiTabLayout::guiTabLayout() {
	// TODO Auto-generated constructor stub
	_xOrigin=0.0;
	_yOrigin=0.0;
	_xOriginTemp=0.0;
	_yOriginTemp=0.0;
}

guiTabLayout::~guiTabLayout() {
	// TODO Auto-generated destructor stub
}


void guiTabLayout::Render(void)
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

void guiTabLayout::ComputeChildAbsPos(void)
{
	float xCurrent = _xOrigin+_xOriginTemp;
	guiBase* pG=_ChildFirst;
	while(pG)
	{
		pG->_x=xCurrent;
		pG->_cx=_cx;

		xCurrent+=_cx;

		pG=pG->_BrotherNext;
	}
}
