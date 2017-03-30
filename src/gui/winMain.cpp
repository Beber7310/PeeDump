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

#include "guiRoot.h"
#include "peeAlbum.h"
#include "peePlaylist.h"
#include "main.h"
#include "winMain.h"
#include "homeControl.h"




guiBase* guiBuild()
{
	guiList* 			albumsWindows 		= new guiList();
	guiList* 			playlistWindows 	= new guiList();
	guiList* 			podcastWindows  	= new guiList();
	guiList* 			homecontrolWindows  = new guiList();
	guiPlayer* 			playerWindows   	= new guiPlayer();
	guiTabLayout*		tabLayout			= new guiTabLayout();
	guiVerticalSplit*	verticalSplit		= new guiVerticalSplit();

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

	homecontrolWindows->AddChild(new guiCourant());
	homecontrolWindows->AddChild(new guiThermo(HC_TEMP_EXTERIEUR));
	homecontrolWindows->AddChild(new guiHeater(HC_HEATER_SALON));
	homecontrolWindows->AddChild(new guiHeater(HC_HEATER_CUISINE));
	homecontrolWindows->AddChild(new guiHeater(HC_HEATER_BARNABE));
	homecontrolWindows->AddChild(new guiHeater(HC_HEATER_DAPHNEE));
	homecontrolWindows->AddChild(new guiThermo(HC_TEMP_VICTOR));
	homecontrolWindows->AddChild(new guiThermo(HC_TEMP_PARENT));
	homecontrolWindows->AddChild(new guiHeater(HC_HEATER_HOMECINEMA));
	homecontrolWindows->AddChild(new guiThermo(HC_TEMP_GARAGE));



	playlistWindows->SetName("Playlists");
	albumsWindows->SetName("Albums");
	podcastWindows->SetName("Podcasts");
	playerWindows->SetName("Player");
	homecontrolWindows->SetName("Home Control");

	//tabLayout->AddChild(playerWindows);
	tabLayout->AddChild(playlistWindows);
	tabLayout->AddChild(albumsWindows);
	tabLayout->AddChild(podcastWindows);
	tabLayout->AddChild(homecontrolWindows);

	verticalSplit->AddChild(tabLayout);
	verticalSplit->AddChild(playerWindows);

	return verticalSplit;
}


int winLaunch()
{
	guiLaunch();
	guiSetMainWindows(guiBuild());
	//pthread_exit(NULL);
	return 0;
}
