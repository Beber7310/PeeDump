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
#include "peeAlbum.h"
#include "peePlaylist.h"

#include "configuration.h"

stAppContext appContext;

int main(int argc, char *argv[]) {

	system("pactl unload-module module-combine-sink");
	system("pulseaudio -D");
	//system("pactl load-module module-combine-sink   sink_name=record-n-play slaves=alsa_output.platform-soc_sound.analog-stereo");

	toolsDownloadInit();
	system("mpc update");

	// First of all we retrieve user id and token
	appContext.gUser = toolsGetUser();
	toolsGetToken(appContext.gToken);

	// fetch album list
	appContext.Albums = toolsGetUserAlbums(appContext.gUser);
	appContext.Playlist = toolsGetUserPlaylists(appContext.gUser);

	/*
	 appContext.Albums=toolsGetUserAlbums(2611892242ul);
	 appContext.Playlist=toolsGetUserPlaylists(2611892242ul);
	 */

	deezerLaunch(appContext.gToken);
	while (!deezerIsReady()) {
		sleep(1);
	}

	while(1)
	{
		// fetch album list
		appContext.Albums = toolsGetUserAlbums(appContext.gUser);
		appContext.Playlist = toolsGetUserPlaylists(appContext.gUser);

		toolsDownloaderTracks(appContext.Albums);
		toolsDownloaderPlaylist(appContext.Playlist);

		while(appContext.Albums->size()) {
			delete (appContext.Albums->back());
			appContext.Albums->pop_back();
		}
		appContext.Albums->clear();

		while(appContext.Playlist->size()) {
			delete (appContext.Playlist->back());
			appContext.Playlist->pop_back();
		}
		appContext.Playlist->clear();

		system("mpc update");
		printf("\n          Deezer download done! restart in 30s\n");

		sleep(30);
	}
}

