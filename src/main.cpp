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

#include "http_fetcher.h"		/* Must include this to use HTTP Fetcher */
#include "tools.h"
#include "deezer.h"
#include "main.h"


stAppContext appContext;


int guiLaunch();

int main(int argc, char *argv[]) {



	// First of all we retrieve user id and token
	appContext.gUser=toolsGetUser();
	toolsGetToken(appContext.gToken);

	// fetch album list
	appContext.Albums=toolsGetUserAlbums(appContext.gUser);
	appContext.Playlist=toolsGetUserPlaylists(appContext.gUser);
	appContext.Podcasts=toolsGetUserPodcasts(appContext.gUser);

	toolsPrintAlbums(appContext.Albums);
	toolsPrintPlaylists(appContext.Playlist);
	toolsPrintPodcasts(appContext.Podcasts);

	guiLaunch();

	deezerLaunch(appContext.gToken);

	while(!toolsGetNext(&appContext));
}



