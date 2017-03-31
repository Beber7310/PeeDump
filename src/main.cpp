/*
 * main.c
 *
 *  Created on: 10 mars 2017
 *      Author: Bertrand
 */


#include <stdio.h>
#include <stdint.h>
#include <list>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

#include "http_fetcher.h"		/* Must include this to use HTTP Fetcher */
#include "tools.h"
#include "deezer.h"
#include "main.h"
#include "downloader.h"
#include "homeControl.h"
#include "gui/winMain.h"

stAppContext appContext;


int main(int argc, char *argv[]) {

	homeControl* pHome=new homeControl();


	pHome->refreshData();
	system("pulseaudio -D");
	toolsDownloadInit();
	system("mpc update");

	// First of all we retrieve user id and token
	appContext.gUser=toolsGetUser();
	toolsGetToken(appContext.gToken);
	appContext.Podcasts=toolsGetPodcast();

	// fetch album list
	appContext.Albums=toolsGetUserAlbums(appContext.gUser);
	appContext.Playlist=toolsGetUserPlaylists(appContext.gUser);
	//appContext.Podcasts=toolsGetUserPodcasts(appContext.gUser);


	//toolsPrintAlbums(appContext.Albums);
	//toolsPrintPlaylists(appContext.Playlist);


	winLaunch();

	deezerLaunch(appContext.gToken);

	homeControlLaunch();

	//while(!toolsGetNext(&appContext));

	while(1)
	{
		int ii;

		for(ii=0;ii<appContext.Podcasts->size();ii++)
		{
			appContext.Podcasts->at(ii)->updatePodcast();
		}
		sleep(15*60);

	}
}



