/*
 * guiButton.h
 *
 *  Created on: 13 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_guiButton_H_
#define GUI_guiButton_H_

#include "guiBase.h"


using namespace std;

class guiButton: public guiBase {
public:
	guiButton();
	guiButton(char* text,int size);
	virtual ~guiButton();
	void SetText(char* text);
	virtual void Render(void);
	virtual void Mouse(stMouse* pMouse);

	int _color;
	int _policeSize;
	char* _text;

};

#endif /* GUI_guiButton_H_ */
