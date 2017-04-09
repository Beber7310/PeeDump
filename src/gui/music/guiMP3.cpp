/*
 * guiMP3.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <stddef.h>
#include "guiMP3.h"
#include "deezer.h"

guiMP3::guiMP3() {
	// TODO Auto-generated constructor stub
	_cy=200;
	_dir=NULL;
	_selected=false;
	_color=232;
}


guiMP3::guiMP3(char* dir) {
	_cy=100;
	_dir=dir;
	_selected=false;
	_color=232;

	_dir=(char*)malloc(strlen(dir)+1);;
	strcpy(_dir,dir);

}

guiMP3::~guiMP3() {
	// TODO Auto-generated destructor stub
}

void guiMP3::Render(void)
{
	_color++;
	if(_color>232)
		_color=232;

	ovgFill(44, 77, _color, 1);


	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);
	ovgFill(255, 255, 255, 1);
	if(_dir)
	{
		RenderText(_x+5,_y+5,_dir,25);
	}

}

void guiMP3::Mouse(stMouse* pMouse)
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
			_selected=true;

			deezerPostCommand(DEEZER_CMD_LOAD_DIR_MP3,_dir,_dir);
		}


	}

}
