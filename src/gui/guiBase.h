/*
 * GuiBase.h
 *
 *  Created on: 12 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_GUIBASE_H_
#define GUI_GUIBASE_H_

#include <vector>
#include "shapes.h"
#include "tools.h"

class guiBase {
public:
	guiBase();
	virtual ~guiBase();
	virtual void Render(void);
	void Resize(float x, float y, float w, float h);
	virtual void Mouse(stMouse* pMouse);
	bool MouseIsInWindows(stMouse* pMouse);
	void AddChild(guiBase* pGui);
//protected:
	int _x,_y,_cx,_cy;
	int screenSizeX,screenSizeY;
	int mouseOriginX,mouseOriginY;
	bool mouseFirstTouch;

	guiBase* _BrotherNext;
	guiBase* _BrotherPrev;
	guiBase* _ChildFirst;

	void RenderRoundRect(int x, int y, int w, int h, int rw, int rh);
	void RenderText(int x, int y, const char *s, int pointsize);

	//std::list<GuiBase*> _child;
};

#endif /* GUI_GUIBASE_H_ */
