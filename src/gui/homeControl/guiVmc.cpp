/*
 * guiVmc.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <guiLightPopup.h>
#include <stddef.h>
#include "guiVmc.h"
#include "guiRoot.h"
#include "homeControl.h"

guiVmc::guiVmc() {
	// TODO Auto-generated constructor stub
	_cy=100;
	_color=232;

}

guiVmc::guiVmc(int id) {
	_cy=100;
	_color=232;
	_id=id;
}

guiVmc::~guiVmc() {
	// TODO Auto-generated destructor stub
}

void guiVmc::Render(void)
{
	char szBuf[128];
	_color+=2;
		if(_color>232)
			_color=232;
	ovgFill(44, 77, _color, 1);


	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);

	RenderText(_x+5,_y+15, hcGetLightName(_id),25);

}

void guiVmc::Mouse(stMouse* pMouse)
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
			guiLightPopup*	absolute		= new guiLightPopup(_id);
			guiPopup(absolute);
		}
	}
}
