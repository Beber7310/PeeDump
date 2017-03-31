/*
 * guiHeater.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <guiHeaterPopup.h>
#include <stddef.h>
#include "guiHeater.h"
#include "guiButton.h"
#include "homeControl.h"

#include "guiRoot.h"

guiHeater::guiHeater() {
	// TODO Auto-generated constructor stub
	_cy=100;
	_color=232;
	_id=0;
}

guiHeater::guiHeater(int id) {
	_cy=100;
	_color=232;
	_id=id;
}

guiHeater::~guiHeater() {
	// TODO Auto-generated destructor stub
}

void guiHeater::Render(void)
{
	char szBuf[128];
	_color+=2;
	if(_color>232)
		_color=232;

	if(hcGetHeating(_id))
	{
		ovgFill(200, 77, _color, 1);
	}
	else
	{
		ovgFill(44, 77, _color, 1);
	}


	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);


	RenderText(_x+5,_y+15,hcGetName(_id),25);

	sprintf(szBuf,"%2.1f",hcGetTemp(_id));
	RenderText(_x+450,_y+15,szBuf,40);

	sprintf(szBuf,"%2.1f",hcGetTargetTemp(_id));
	RenderText(_x+40,_y+55,szBuf,20);

}

void guiHeater::Mouse(stMouse* pMouse)
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
			CreatePopup();
		}
	}
}

void guiHeater::CreatePopup()
{
	guiHeaterPopup*	absolute		= new guiHeaterPopup(_id);


	guiPopup(absolute);
}
