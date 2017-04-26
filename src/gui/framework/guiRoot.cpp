/*
 * guiRoot.cpp
 *
 *  Created on: 18 mars 2017
 *      Author: Bertrand
 */
// first OpenVG program
#include "configuration.h"

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


static pthread_mutex_t mousseLock;
static sem_t semaphorDraw;

static stMouse gMouse;

#define EVENT_DEVICE    "/dev/input/event0"

guiBase* currentWindows=NULL;
guiBase* mainWindows=NULL;
guiBase* popupWindows=NULL;
int screenWidth=200, screenHeight=200;



unsigned long lasttime = 0;

void* guiThread(void * p) {

	stMouse localMouse;
	int needDraw=25*5;
	bool	toggle=false;
	struct timeval tv;
	unsigned long lasttime;
	unsigned long microseconds;
	struct timespec ts;

	int lastUpdate=0;

	ovginit(&screenWidth, &screenHeight,NULL);				   // Graphics initialization

	//guiBase* mainWindows=guiBuild();
	currentWindows->Resize(0,0,screenWidth,screenHeight);
	currentWindows->SetScreenSize(screenWidth,screenHeight);

	while(1)
	{
		//ovgFill(44, 77, 232, 1);				   // Big blue marble

		if(currentWindows)
		{
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec +=1;
			sem_timedwait(&semaphorDraw,&ts);
			pthread_mutex_lock(&mousseLock);
			if(((stMouse*) p)->update!=lastUpdate)
			{
				needDraw=25*2;
				localMouse.x=((stMouse*) p)->x;
				localMouse.y=((stMouse*) p)->y;
				localMouse.t=((stMouse*) p)->t;

				currentWindows->Mouse(&localMouse);
				lastUpdate=((stMouse*) p)->update;
			}
			pthread_mutex_unlock(&mousseLock);

			if(needDraw>0)
			{
				needDraw--;
				ovgStart(screenWidth, screenHeight);				   // Start the picture
				ovgBackground(0, 0, 0);				   // Black background
				currentWindows->Render();

				toggle=!toggle;
				ovgFill(toggle?255:0,255,255,1);

				ovgCircle(localMouse.x,screenHeight-localMouse.y,10.0f);
				ovgEnd(); //Moved in callback
			}
		}
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
	pMouse->update=0;

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

#ifdef	SCREEN_7P
			pMouse->x=600-y; // swap on purpose !!!
			pMouse->y=x; // swap on purpose !!!
#elif defined(SCREEN_5P)
			pMouse->x=480.0f*(y-250.0f)/3600.0f; // swap on purpose !!!
			pMouse->y=800-(800.0f*(x-154.0f)/3800.0f); // swap on purpose !!!
#else
	#error
#endif

			pMouse->t=t;
			pMouse->update++;
			//printf("%3.3i %3.3i %1.1i\n",pMouse->x, pMouse->y,pMouse->t);
			pthread_mutex_unlock(&mousseLock);
			guiInvalidate();
		}
	}

	return 0;

	err:
	close(fd);
	return 0;
}

void guiInvalidate()
{
	sem_post(&semaphorDraw);
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


	sem_init(&semaphorDraw,0,1);

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
	pWin->Resize(0,0,screenWidth,screenHeight);
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



