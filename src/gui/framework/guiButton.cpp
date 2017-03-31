/*
 * guiButton.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <guiButton.h>
#include <guiRoot.h>
#include <stddef.h>
#include "homeControl.h"
#include "stdlib.h"

guiButton::guiButton() {
	// TODO Auto-generated constructor stub
	_cy=100;
	_color=232;
	_text=NULL;
	_policeSize=12;
}

guiButton::guiButton(char* text,int size) {
	_cy=100;
	_color=232;
	SetText(text);
	_policeSize=size;
}


guiButton::~guiButton() {
	// TODO Auto-generated destructor stub
}

void guiButton::SetText(char* text)
{
	_text=(char*)malloc(strlen(text)+1);
	strcpy(_text,text);
}

void guiButton::Render(void)
{
	_color++;
		if(_color>232)
			_color=232;
	ovgFill(44, 77, _color, 1);

	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);

	RenderTextMid(_x+_cx/2,(_y+_cy/2)-_policeSize/2,_text,_policeSize);
}

void guiButton::Mouse(stMouse* pMouse)
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
