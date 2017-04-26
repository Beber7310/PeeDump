/*
 * GuiTabLayout.cpp
 *
 *  Created on: 12 mars 2017
 *      Author: Bertrand
 */

#include "guiTabLayout.h"
#include "guiRoot.h"
#include <math.h>

guiTabLayout::guiTabLayout() {
	// TODO Auto-generated constructor stub
	_xOrigin=0.0;
	_yOrigin=0.0;
	_xOriginTemp=0.0;
	_yOriginTemp=0.0;
	_titleHeight=100.0f;
}

guiTabLayout::~guiTabLayout() {
	// TODO Auto-generated destructor stub
}


void guiTabLayout::Render(void)
{


	guiBase* pG=_ChildFirst;

	if(_xTarget==_xOrigin)
	{

	}
	else if(abs(_xTarget-_xOrigin)<100)
	{
		_xOrigin=_xTarget;
		guiInvalidate();
	}
	else if(_xTarget>_xOrigin)
	{
		_xOrigin+=80;
		guiInvalidate();
	}
	else
	{
		_xOrigin-=80;
		guiInvalidate();
	}

	ComputeChildAbsPos();

	while(pG)
	{
		bool skip=false;

		if(pG->_x>=_cx)
			skip=true;
		if((pG->_x+pG->_cx)<=_x)
			skip=true;
		if(!skip)
			pG->Render();

		// Title
		ovgFill(0, 0, 50, 1);
		RenderRoundRect(pG->_x,_y,_cx,_titleHeight,30,30);
		ovgFill(0, 0, 80, 1);
		RenderRoundRect(pG->_x+10,_y+10,_cx-20,_titleHeight-20,30,30);
		ovgFill(255, 255, 255, 1);
		RenderText(pG->_x+20,_y+20,pG->_windowName,40);

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
		pG->_y=_y +_titleHeight;
		pG->_cx=_cx;
		pG->_cy=_cy - _titleHeight;
		xCurrent+=_cx;

		pG=pG->_BrotherNext;
	}
}

void guiTabLayout::Mouse(stMouse* pMouse)
{

	guiBase::Mouse(pMouse);
	if(MouseIsInWindows(pMouse))
	{
		if(pMouse->t)
		{
			_xOriginTemp=pMouse->x-mouseOriginX;
			_xOriginTemp=(_xOrigin+_xOriginTemp>0)?0:_xOriginTemp;
			_xOriginTemp=(_xOrigin+_xOriginTemp<(-_cx*(nbrChild-1)))?0:_xOriginTemp;

			if(abs(pMouse->x-mouseOriginX)<50)
				_xOriginTemp=0;
		}
		else if(pMouse->LastTouch)
		{
			if((pMouse->x-mouseOriginX)>100)
			{
				_xTarget=_xOrigin+_cx;
			}
			if((pMouse->x-mouseOriginX)<-100)
			{
				_xTarget=_xOrigin-_cx;
			}


			_xTarget=(_xTarget>0)?0:_xTarget;
			_xTarget=(_xTarget<(-_cx*(nbrChild-1)))?(-_cx*(nbrChild-1)):_xTarget;

			_xOrigin+=_xOriginTemp;
			_xOriginTemp=0;

		}
		else
		{

		}




		guiBase* pG=_ChildFirst;
		while(pG)
		{
			pG->Mouse(pMouse);
			pG=pG->_BrotherNext;
		}
	}

}

