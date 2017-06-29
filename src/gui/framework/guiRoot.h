/*
 * guiRoot.h
 *
 *  Created on: 18 mars 2017
 *      Author: Bertrand
 */

#ifndef GUI_FRAMEWORK_GUIROOT_H_
#define GUI_FRAMEWORK_GUIROOT_H_

#include "guiBase.h"

void guiInit(void);
int guiLaunch(void);
int guiSetMainWindows(guiBase* pWin);

int guiSetMainWindows(guiBase* pWin);
int guiPopup(guiBase* pWin);
int guiExitPopup();
void guiInvalidate();

#endif /* GUI_FRAMEWORK_GUIROOT_H_ */
