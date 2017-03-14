/*
 * GuiBase.cpp
 *
 *  Created on: 12 mars 2017
 *      Author: Bertrand
 */

#include "guiBase.h"

#include "Shapes.h"


guiBase::guiBase() {
	// TODO Auto-generated constructor stub
	_x=0;
	_y=0;
	_cx=500;
	_cy=100;

	_BrotherNext=NULL;
	_BrotherPrev=NULL;
	_ChildFirst=NULL;

	screenSizeX=600;
	screenSizeY=1024;
	mouseOriginX=0;
	mouseOriginY=0;
	mouseFirstTouch=true;
}

guiBase::~guiBase() {
	// TODO Auto-generated destructor stub
}

void guiBase::Render(void)
{
	RenderRoundRect(_x, _y, _cx, _cy, 5, 5);
}

void guiBase::Resize(float x, float y, float w, float h)
{
	_x=x;
	_y=y;
	_cx=w;
	_cy=h;
}

void guiBase::AddChild(guiBase* pGui)
{
	guiBase* pG;
	if(_ChildFirst==NULL)
		_ChildFirst=pGui;
	else
	{
		pG=_ChildFirst;
		while(pG->_BrotherNext)
		{
			pG=pG->_BrotherNext;
		}
		pG->_BrotherNext=pGui;
	}
}

void guiBase::Mouse(stMouse* pMouse)
{
	pMouse->firstTouch=false;
	pMouse->LastTouch=false;

	// the finger touch the screen fot the first time
	if(pMouse->t==true && mouseFirstTouch==true)
	{
		mouseOriginX=pMouse->x;
		mouseOriginY=pMouse->y;
		mouseFirstTouch=false;
		pMouse->firstTouch=true;
		pMouse->Click=false;
	}else if(pMouse->t==false && mouseFirstTouch==false) // the finger leave the screen
	{
		mouseFirstTouch=true;
		pMouse->LastTouch=true;
		if((abs(pMouse->x-mouseOriginX)<5) && (abs(pMouse->y-mouseOriginY)<5))
			pMouse->Click=true;
	}else if(pMouse->t==false && mouseFirstTouch==true)// no finger on the screen
	{
		pMouse->Click=false;
	}
}

bool guiBase::MouseIsInWindows(stMouse* pMouse)
{
	if(pMouse->x<_x)
		return false;
	if(pMouse->y<_y)
		return false;
	if(pMouse->x>(_x+_cx))
		return false;
	if(pMouse->y>(_y+_cy))
		return false;
	return true;
}

void guiBase::RenderRoundRect(int x, int y, int w, int h, int rw, int rh)
{
	float fx=x;
	float fy=((screenSizeY-y)-h);
	float fw=w;
	float fh=h;
	float frw=rw;
	float frh=rh;

	ovgRoundrect( fx,  fy,  fw,  fh,  frw,  frh);
}

void guiBase::RenderText(int x, int y, const char *s, int pointsize)
{
	ovgText( x,  (screenSizeY-y)-pointsize, s,  SerifTypeface,  pointsize);

}


