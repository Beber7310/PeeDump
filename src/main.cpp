/*
 * main.c
 *
 *  Created on: 10 mars 2017
 *      Author: Bertrand
 */


#include <stdio.h>
#include <stdint.h>
#include <list>
#include "http_fetcher.h"		/* Must include this to use HTTP Fetcher */
#include "tools.h"
#include "deezer.h"

char 		gToken[128];
uint32_t	gUser;
std::list<peeAlbum*>*   gAlbums;
std::list<peePlaylist*>*   gPlaylist;

int main(int argc, char *argv[]) {


	// First of all we retrieve user id and token
	gUser=toolsGetUser();
	toolsGetToken(gToken);

	// fetch album list
	gAlbums=toolsGetUserAlbums(gUser);
	gPlaylist=toolsGetUserPlaylists(gUser);

	toolsPrintAlbums(gAlbums);
	toolsPrintPlaylists(gPlaylist);

	deezerLaunch(gToken);

	while(!toolsGetNext());
}

