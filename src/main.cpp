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

#include "configuration.h"

stAppContext appContext;


int main(int argc, char *argv[]) {

	system("pactl unload-module module-combine-sink");
	system("pulseaudio -D");
	system("pactl load-module module-combine-sink   sink_name=record-n-play slaves=alsa_output.platform-soc_sound.analog-stereo");

	toolsDownloadInit();
	system("mpc update");

	// First of all we retrieve user id and token
	appContext.gUser=toolsGetUser();
	toolsGetToken(appContext.gToken);

	// fetch album list
	appContext.Albums=toolsGetUserAlbums(appContext.gUser);
	appContext.Playlist=toolsGetUserPlaylists(appContext.gUser);

	deezerLaunch(appContext.gToken);
	while(!deezerIsReady())
	{
		sleep(1);
	}
	toolsDownloaderTracks(appContext.Albums);
	printf("\n          Deezer download done!\n");
	//while(!toolsGetNext(&appContext));


}



