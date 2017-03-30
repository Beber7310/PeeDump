/*
 * guiPlayer.cpp
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */
#include <stddef.h>
#include "guiPlayer.h"
#include "deezer.h"

#define ARROW_X_0	0
#define ARROW_X_1	60
#define ARROW_X_2	0

#define ARROW_Y_0	0
#define ARROW_Y_1	30
#define ARROW_Y_2	60

#define OFFSET_NEXT_X	400
#define OFFSET_NEXT_Y	12

#define OFFSET_PREV_X	200
#define OFFSET_PREV_Y	12


#define PAUSE_LEFT		0
#define PAUSE_RIGHT		25
#define PAUSE_TOP		0
#define PAUSE_BOTTOM	60

#define OFFSET_PAUSE_X	310
#define OFFSET_PAUSE2_X	(290-PAUSE_RIGHT)
#define OFFSET_PAUSE_Y	12


guiPlayer::guiPlayer() {
	// TODO Auto-generated constructor stub
	_cy=600;
	colorNext=255;
	colorPrev=255;
	colorPause=255;
}

guiPlayer::~guiPlayer() {
	// TODO Auto-generated destructor stub
}

void guiPlayer::Render(void)
{
	ovgFill(232, 77, 44 , 1);
	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);

	ovgFill(255, colorNext, 255, 1);
	float NextX[3] ={OFFSET_NEXT_X+_x+ARROW_X_0,OFFSET_NEXT_X+_x+ARROW_X_1,OFFSET_NEXT_X+_x+ARROW_X_2};
	float NextY[3] ={OFFSET_NEXT_Y+_y+ARROW_Y_0,OFFSET_NEXT_Y+_y+ARROW_Y_1,OFFSET_NEXT_Y+_y+ARROW_Y_2};
	Polygon(NextX,NextY,3);

	ovgFill(255, colorPrev, 255, 1);
	float PrevX[3] ={OFFSET_PREV_X+_x+ARROW_X_0,OFFSET_PREV_X+_x-ARROW_X_1,OFFSET_PREV_X+_x+ARROW_X_2};
	float PrevY[3] ={OFFSET_PREV_Y+_y+ARROW_Y_0,OFFSET_PREV_Y+_y+ARROW_Y_1,OFFSET_PREV_Y+_y+ARROW_Y_2};
	Polygon(PrevX,PrevY,3);

	ovgFill(255, colorPause, 255, 1);
	float PauseX[4]={OFFSET_PAUSE_X+_x+PAUSE_LEFT ,OFFSET_PAUSE_X+_x+PAUSE_RIGHT	,OFFSET_PAUSE_X+_x+PAUSE_RIGHT		,OFFSET_PAUSE_X+_x+PAUSE_LEFT};
	float PauseY[4]={OFFSET_PAUSE_Y+_y+PAUSE_TOP  ,OFFSET_PAUSE_Y+_y+PAUSE_TOP		,OFFSET_PAUSE_Y+_y+PAUSE_BOTTOM		,OFFSET_PAUSE_Y+_y+PAUSE_BOTTOM};

	float PauseX2[4]={OFFSET_PAUSE2_X+_x+PAUSE_LEFT ,OFFSET_PAUSE2_X+_x+PAUSE_RIGHT	,OFFSET_PAUSE2_X+_x+PAUSE_RIGHT		,OFFSET_PAUSE2_X+_x+PAUSE_LEFT};
	float PauseY2[4]={OFFSET_PAUSE_Y+_y+PAUSE_TOP  ,OFFSET_PAUSE_Y+_y+PAUSE_TOP		,OFFSET_PAUSE_Y+_y+PAUSE_BOTTOM		,OFFSET_PAUSE_Y+_y+PAUSE_BOTTOM};


	Polygon(PauseX,PauseY,4);
	Polygon(PauseX2,PauseY2,4);

	if(colorNext<254)
		colorNext+=2;

	if(colorPrev<254)
		colorPrev+=2;

	if(colorPause<254)
		colorPause+=2;

	RenderText(_x+20,_y+95,deezerGetSongName(),15);
}

void guiPlayer::Mouse(stMouse* pMouse)
{
	guiBase::Mouse(pMouse);
	if(MouseIsInWindows(pMouse))
	{
		if(pMouse->t)
		{
			if(	((pMouse->x-_x)>OFFSET_NEXT_X) && (pMouse->x-_x)<(OFFSET_NEXT_X+ARROW_X_1)&&
					((pMouse->y-_y)>OFFSET_NEXT_Y) && (pMouse->y-_y)<(OFFSET_NEXT_Y+ARROW_Y_2))
			{
				colorNext=128;
			}

			if(	((pMouse->x-_x)>(OFFSET_PREV_X-ARROW_X_1)) && (pMouse->x-_x)<(OFFSET_PREV_X)&&
					((pMouse->y-_y)>OFFSET_PREV_Y) && (pMouse->y-_y)<(OFFSET_PREV_Y+ARROW_Y_2))
			{

				colorPrev=128;

			}
			if(	((pMouse->x-_x)>(OFFSET_PAUSE2_X-PAUSE_LEFT)) && (pMouse->x-_x)<(OFFSET_PAUSE_X+PAUSE_RIGHT)&&
					((pMouse->y-_y)>OFFSET_PAUSE_Y) && (pMouse->y-_y)<(OFFSET_PAUSE_Y+PAUSE_BOTTOM))
			{

				colorPause=128;

			}
		}
		else if(pMouse->Click)
		{
			if(	((pMouse->x-_x)>OFFSET_NEXT_X) && (pMouse->x-_x)<(OFFSET_NEXT_X+ARROW_X_1)&&
					((pMouse->y-_y)>OFFSET_NEXT_Y) && (pMouse->y-_y)<(OFFSET_NEXT_Y+ARROW_Y_2))
			{
				colorNext=128;
				deezerPostCommand(DEEZER_CMD_NEXT ,0,0);
			}

			if(	((pMouse->x-_x)>(OFFSET_PREV_X-ARROW_X_1)) && (pMouse->x-_x)<(OFFSET_PREV_X)&&
					((pMouse->y-_y)>OFFSET_PREV_Y) && (pMouse->y-_y)<(OFFSET_PREV_Y+ARROW_Y_2))
			{
				colorPrev=128;
				deezerPostCommand(DEEZER_CMD_PREV ,0,0);
			}
			if(	((pMouse->x-_x)>(OFFSET_PAUSE2_X-PAUSE_LEFT)) && (pMouse->x-_x)<(OFFSET_PAUSE_X+PAUSE_RIGHT)&&
					((pMouse->y-_y)>OFFSET_PAUSE_Y) && (pMouse->y-_y)<(OFFSET_PAUSE_Y+PAUSE_BOTTOM))
			{
				deezerPostCommand(DEEZER_CMD_PAUSE ,0,0);
				colorPause=128;

			}
		}


	}

}
