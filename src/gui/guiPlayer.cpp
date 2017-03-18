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
#define ARROW_X_1	100
#define ARROW_X_2	0

#define ARROW_Y_0	0
#define ARROW_Y_1	50
#define ARROW_Y_2	100

#define OFFSET_NEXT_X	400
#define OFFSET_NEXT_Y	50

#define OFFSET_PREV_X	200
#define OFFSET_PREV_Y	50


guiPlayer::guiPlayer() {
	// TODO Auto-generated constructor stub
	_cy=600;
	colorNext=255;
	colorPrev=255;
}

guiPlayer::~guiPlayer() {
	// TODO Auto-generated destructor stub
}

void guiPlayer::Render(void)
{
	ovgFill(44, 77, 232, 1);
	RenderRoundRect(_x, _y, _cx, _cy, 25, 25);

	ovgFill(255, colorNext, 255, 1);
	float NextX[3] ={OFFSET_NEXT_X+_x+ARROW_X_0,OFFSET_NEXT_X+_x+ARROW_X_1,OFFSET_NEXT_X+_x+ARROW_X_2};
	float NextY[3] ={OFFSET_NEXT_Y+_y+ARROW_Y_0,OFFSET_NEXT_Y+_y+ARROW_Y_1,OFFSET_NEXT_Y+_y+ARROW_Y_2};
	Polygon(NextX,NextY,3);

	ovgFill(255, colorPrev, 255, 1);
	float PrevX[3] ={OFFSET_PREV_X+_x+ARROW_X_0,OFFSET_PREV_X+_x-ARROW_X_1,OFFSET_PREV_X+_x+ARROW_X_2};
	float PrevY[3] ={OFFSET_PREV_Y+_y+ARROW_Y_0,OFFSET_PREV_Y+_y+ARROW_Y_1,OFFSET_PREV_Y+_y+ARROW_Y_2};
	Polygon(PrevX,PrevY,3);

	if(colorNext<254)
		colorNext+=2;

	if(colorPrev<254)
		colorPrev+=2;
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
		}
		else if(pMouse->Click)
		{
			if(	((pMouse->x-_x)>OFFSET_NEXT_X) && (pMouse->x-_x)<(OFFSET_NEXT_X+ARROW_X_1)&&
					((pMouse->y-_y)>OFFSET_NEXT_Y) && (pMouse->y-_y)<(OFFSET_NEXT_Y+ARROW_Y_2))
			{
				printf("Next!\n");
				colorNext=128;
				deezerPostCommand(DEEZER_CMD_NEXT ,0);
			}

			if(	((pMouse->x-_x)>(OFFSET_PREV_X-ARROW_X_1)) && (pMouse->x-_x)<(OFFSET_PREV_X)&&
					((pMouse->y-_y)>OFFSET_PREV_Y) && (pMouse->y-_y)<(OFFSET_PREV_Y+ARROW_Y_2))
			{
				printf("Prev!\n");
				colorPrev=128;
				deezerPostCommand(DEEZER_CMD_PREV ,0);
			}
		}


	}

}
