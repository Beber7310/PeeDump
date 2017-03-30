/*
 * guiCourant.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <stddef.h>
#include "guiCourant.h"
#include "homeControl.h"

guiCourant::guiCourant() {
	// TODO Auto-generated constructor stub
	_cy=100;
	_color=232;
}

guiCourant::guiCourant(int id) {
	_cy=100;
	_color=232;
}

guiCourant::~guiCourant() {
	// TODO Auto-generated destructor stub
}

void guiCourant::Render(void)
{
	char szBuf[128];
	_color++;
		if(_color>232)
			_color=232;
	ovgFill(44, 77, _color, 1);


	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);

	sprintf(szBuf,"%2.1f",hcGetCourant());
	RenderText(_x+5,_y+5,"Courant",25);
	RenderText(_x+450,_y+5,szBuf,40);
}

void guiCourant::Mouse(stMouse* pMouse)
{
	guiBase::Mouse(pMouse);
	if(MouseIsInWindows(pMouse))
	{
		if(pMouse->t)
		{

		}
		else if(pMouse->Click)
		{
			_color=100;
		}
	}
}
