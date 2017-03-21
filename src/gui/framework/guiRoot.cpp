/*
 * guiRoot.cpp
 *
 *  Created on: 18 mars 2017
 *      Author: Bertrand
 */
// first OpenVG program
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#include "EGL/egl.h"


#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"

#include <pthread.h>
#include <semaphore.h>

#include "main.h"

#include "guiBase.h"
#include "guiRoot.h"


static pthread_mutex_t vSynclock;
static pthread_mutex_t mousseLock;
static stMouse gMouse;

#define EVENT_DEVICE    "/dev/input/event0"

guiBase* currentWindows=NULL;
guiBase* mainWindows=NULL;
guiBase* popupWindows=NULL;
int screenWidth=200, screenHeight=200;

void guiVSyncCallBack(DISPMANX_UPDATE_HANDLE_T u, void * arg)
{
	pthread_mutex_unlock(&vSynclock);
}

unsigned long lasttime = 0;

void* guiThread(void * p) {

	stMouse localMouse;
	struct timeval tv;
	unsigned long lasttime;
	unsigned long microseconds;
	ovginit(&screenWidth, &screenHeight,&guiVSyncCallBack);				   // Graphics initialization

	//guiBase* mainWindows=guiBuild();
	currentWindows->Resize(0,0,screenWidth,screenHeight);


	while(1)
	{

		gettimeofday(&tv, NULL);
		lasttime = (tv.tv_sec*1000000)+tv.tv_usec;

		pthread_mutex_lock(&mousseLock);
		localMouse.x=((stMouse*) p)->x;
		localMouse.y=((stMouse*) p)->y;
		localMouse.t=((stMouse*) p)->t;
		pthread_mutex_unlock(&mousseLock);

		ovgStart(screenWidth, screenHeight);				   // Start the picture

		ovgBackground(0, 0, 0);				   // Black background
		//ovgFill(44, 77, 232, 1);				   // Big blue marble

		if(currentWindows)
		{
			currentWindows->Mouse(&localMouse);
			currentWindows->Render();
		}
		//pthread_mutex_lock(&vSynclock);
		gettimeofday(&tv, NULL);
		microseconds = (tv.tv_sec*1000000)+tv.tv_usec;

		//printf("sync  %lu \n", microseconds-lasttime);


		ovgEnd(); //Moved in callback						   // End the picture

	}

}

void* guiMouseThread(void * p)
{
	stMouse* pMouse;
	int x=0,y=0,t=0;
	struct input_event ev;
	int fd;
	char name[256] = "Unknown";
	pMouse=(stMouse*)p;
	bool update=false;
	/* Open Device */
	fd = open(EVENT_DEVICE, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "%s is not a vaild device\n", EVENT_DEVICE);
		pthread_exit(NULL);
		return 0;
	}

	/* Print Device Name */
	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	printf("Reading from:\n");
	printf("device file = %s\n", EVENT_DEVICE);
	printf("device name = %s\n", name);

	for (;;) {
		const size_t ev_size = sizeof(struct input_event);
		ssize_t size;

		/* TODO: use select() */

		size = read(fd, &ev, ev_size);
		if (size < ev_size) {
			fprintf(stderr, "Error size when reading\n");
			goto err;
		}

		if (ev.type == EV_ABS && ev.code == ABS_X) {
			/* TODO: convert value to pixels */
			//printf("X = %d\n", ev.value);
			x=ev.value;
			update=true;
		}
		if (ev.type == EV_ABS && ev.code == ABS_Y) {
			/* TODO: convert value to pixels */
			//printf("Y = %d\n", ev.value);
			y=ev.value;
			update=true;
		}
		else if(ev.type == EV_KEY && (ev.code == BTN_TOUCH))
		{
			//printf("touch: %i\n", ev.value);
			t=ev.value;
			if(!t)
				update=true;
		}
		else
		{
			//printf("%x %x %x\n",ev.type, ev.code,ev.value);
		}

		if(update)
		{
			update=false;
			pthread_mutex_lock(&mousseLock);
			pMouse->x=600-y; // swap on purpose !!!
			pMouse->y=x; // swap on purpose !!!
			pMouse->t=t;
			pthread_mutex_unlock(&mousseLock);
		}
	}

	return 0;

	err:
	close(fd);
	return 0;
}

int guiLaunch(void)
{
	pthread_t my_guiThread;
	pthread_t my_mouseThread;
	int ret;


	if (pthread_mutex_init(&mousseLock, NULL) != 0)
	{
		printf("\n mousse  mutex init failed\n");
		return 1;
	}

	if (pthread_mutex_init(&vSynclock, NULL) != 0)
	{
		printf("\n vSynclock  mutex init failed\n");
		return 1;
	}

	ret =  pthread_create(&my_guiThread, NULL, &guiThread, &gMouse);
	ret =  pthread_create(&my_mouseThread, NULL, &guiMouseThread, &gMouse);
	if(ret != 0) {
		printf("Error: pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}

	//pthread_exit(NULL);
	return 0;
}

int guiSetMainWindows(guiBase* pWin)
{
	pWin->Resize(0,0,screenWidth,screenHeight);
	currentWindows=pWin;
	mainWindows=pWin;
	return 0;
}

int guiPopup(guiBase* pWin)
{
	pWin->Resize(50,50,screenWidth-100,screenHeight-100);
	currentWindows=pWin;
	popupWindows=pWin;
	return 0;
}

int guiExitPopup()
{
	if(popupWindows)
		free(popupWindows);
	popupWindows=NULL;

	currentWindows=mainWindows;
	return 0;
}



