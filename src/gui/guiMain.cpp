/*
 * guiMain.cpp
 *
 *  Created on: 13 mars 2017
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

#include "guiMain.h"
#include "peeAlbum.h"
#include "peePlaylist.h"
#include "main.h"

static pthread_mutex_t vSynclock;
static pthread_mutex_t mousseLock;
static stMouse gMouse;

#define EVENT_DEVICE    "/dev/input/event0"

unsigned long lasttime = 0;
void guiVSyncCallBack(DISPMANX_UPDATE_HANDLE_T u, void * arg)
{

	pthread_mutex_unlock(&vSynclock);

}

guiBase* guiBuild()
{
	guiList* 		albumsWindows 	= new guiList();
	guiList* 		playlistWindows = new guiList();
	guiList* 		podcastWindows  = new guiList();
	guiPlayer* 		playerWindows   = new guiPlayer();
	guiTabLayout*	tabLayout		= new guiTabLayout();

	for (vector<peeAlbum*>::iterator it = appContext.Albums->begin(); it != appContext.Albums->end(); it++)
	{
		albumsWindows->AddChild(new guiAlbum(*it));
	}

	for (vector<peePlaylist*>::iterator it = appContext.Playlist->begin(); it != appContext.Playlist->end(); it++)
	{
		playlistWindows->AddChild(new guiPlaylist(*it));
	}

	for (vector<peePodcast*>::iterator it = appContext.Podcasts->begin(); it != appContext.Podcasts->end(); it++)
	{
		podcastWindows->AddChild(new guiPodcast(*it));
	}

	playlistWindows->SetName("Playlists");
	albumsWindows->SetName("Albums");
	podcastWindows->SetName("Podcasts");
	playerWindows->SetName("Player");

	tabLayout->AddChild(playerWindows);
	tabLayout->AddChild(playlistWindows);
	tabLayout->AddChild(albumsWindows);
	tabLayout->AddChild(podcastWindows);

	return tabLayout;
}

void* guiThread(void * p) {
	int width, height;
	stMouse localMouse;

	ovginit(&width, &height,&guiVSyncCallBack);				   // Graphics initialization

	guiBase* mainWindows=guiBuild();
	mainWindows->Resize(0,0,width,height);


	while(1)
	{

		pthread_mutex_lock(&mousseLock);
		localMouse.x=((stMouse*) p)->x;
		localMouse.y=((stMouse*) p)->y;
		localMouse.t=((stMouse*) p)->t;
		pthread_mutex_unlock(&mousseLock);

		ovgStart(width, height);				   // Start the picture

		ovgBackground(0, 0, 0);				   // Black background
		//ovgFill(44, 77, 232, 1);				   // Big blue marble

		mainWindows->Mouse(&localMouse);
		mainWindows->Render();

		pthread_mutex_lock(&vSynclock);
		ovgEnd(); //Moved in callback						   // End the picture


		/*
		struct timeval tv;
		gettimeofday(&tv, NULL);
		unsigned long microseconds = (tv.tv_sec*1000000)+tv.tv_usec;
		printf("sync  %lu \n", microseconds-lasttime);
		lasttime = microseconds;
		*/
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


int guiLaunch()
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
