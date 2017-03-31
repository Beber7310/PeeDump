/*
 * guiHeaterPopup.cpp
 *
 *  Created on: 21 mars 2017
 *      Author: Bertrand
 */

#include <guiHeaterPopup.h>
#include <guiButton.h>
#include "homeControl.h"
#include "guiRoot.h"

guiHeaterPopup::guiHeaterPopup(	int id) {
	// TODO Auto-generated constructor stub
	_heaterPlus		= new guiButton("+",80);
	_heaterMoins	= new guiButton("-",80);
	_heaterOk		= new guiButton("OK",50);

	_heaterPlus->_x=350;
	_heaterPlus->_y=350;
	_heaterPlus->_cx=200;
	_heaterPlus->_cy=200;

	_heaterMoins->_x= 50;
	_heaterMoins->_y=350;
	_heaterMoins->_cx=200;
	_heaterMoins->_cy=200;

	_heaterOk->_x=100;
	_heaterOk->_y=800;
	_heaterOk->_cx=400;
	_heaterOk->_cy=100;

	AddChild(_heaterPlus);
	AddChild(_heaterMoins);
	AddChild(_heaterOk);

	_id=id;
}

guiHeaterPopup::~guiHeaterPopup() {
	// TODO Auto-generated destructor stub

}

void guiHeaterPopup::Render(void)
{
	char szBuf[128];
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
	RenderTextMid(_x+_cx/2,_y+100,hcGetName(_id),60);

	sprintf(szBuf,"%2.1f",hcGetTargetTemp(_id));
	RenderTextMid(_x+_cx/2,_y+200,szBuf,60);
}

void guiHeaterPopup::ComputeChildAbsPos(void)
{

}

void guiHeaterPopup::Mouse(stMouse* pMouse)
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
		if(pMouse->Click)
		{
			if(_heaterPlus->MouseIsInWindows(pMouse))
			{
				hcSetTargetTemp(_id,hcGetTargetTemp(_id)+1.0);
			}

			if(_heaterMoins->MouseIsInWindows(pMouse))
			{
				hcSetTargetTemp(_id,hcGetTargetTemp(_id)-1.0);
			}

			if(_heaterOk->MouseIsInWindows(pMouse))
			{
				guiExitPopup();
			}
		}

	}
}

