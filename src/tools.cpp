/*
 * tools.c
 *
 *  Created on: 10 mars 2017
 *      Author: Bertrand
 */

#include "configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include <semaphore.h>
#include <pthread.h>

#include "tinyxml2.h"
#include "http_fetcher.h"
#include "peeAlbum.h"
#include "peePlaylist.h"
#include "peeTrack.h"
#include "peeAlbum.h"
#include "deezer.h"
#include "tools.h"
#include "main.h"
#include "downloader.h"

using namespace tinyxml2;
using namespace std;

void cleanMP3String(char* str)
{
	for(unsigned int ii=0;ii<strlen(str);ii++)
	{
		if(str[ii]=='\"')
			str[ii]='\'';
		if(str[ii]=='`')
			str[ii]='\'';
		if(str[ii]=='\\')
			str[ii]=' ';
		if(str[ii]=='/')
			str[ii]=' ';
	}

}

uint32_t toolsGetUser() {
	uint32_t idUser = 0;

	// return 92847721; //Bertrand
	return 2611892242ul; //Victor
	// return 2607063582; // Daphnée
	// return 3060697284; // Barnabé
}


void toolsGetToken(char* pToken) {

	//pToken = strcpy(pToken, "frxQg2WGFPHlnTxC9dRj9tWnls4Ik53PlxxrTlqEnKMPDeCbvh");//Bertrand
	pToken = strcpy(pToken, "frICC5hssXh2fnzZG8yyQABCxiC3cbmQQHGpA4YsrAWTQsZ0R4");//Victor fro0Lr2bRLO4exEMtvmSfNTQcou1jQdNKX9rwTYF02dAhdTTmi
	//pToken = strcpy(pToken, "fro0Lr2bRLO4exEMtvmSfNTQcou1jQdNKX9rwTYF02dAhdTTmi");//Daphnée frICC5hssXh2fnzZG8yyQABCxiC3cbmQQHGpA4YsrAWTQsZ0R4
	//pToken = strcpy(pToken, "frmNr8YaJXRToOMHlgtteClyMVyaX7DWExPBwT4LEf1821gnbg");//Barnabé
	return;
}

std::vector<peePodcast*>* toolsGetPodcast(void) {
	FILE *stream;
	char *line = NULL;
	char* podCast;
	size_t len = 0;
	ssize_t read;
	std::vector<peePodcast*>*retPodcast = new std::vector<peePodcast*>();

	stream = fopen("/home/pi/projects/data.conf", "r");
	if (stream == NULL) {
		printf("Unable to open /home/pi/projects/data.conf\n");
		exit(EXIT_FAILURE);
	}
	while ((read = getline(&line, &len, stream)) != -1) {
		if (strncmp("lpodcast:", line, strlen("lpodcast:")) == 0) {
			podCast = &line[strlen("lpodcast:")];
			podCast[strlen(podCast) - 1] = 0;		// remove last char as it is a \n

			retPodcast->push_back(new peePodcast(podCast, 100, 10));
		}
		if (strncmp("spodcast:", line, strlen("spodcast:")) == 0) {
			podCast = &line[strlen("spodcast:")];
			podCast[strlen(podCast) - 1] = 0;		// remove last char as it is a \n

			retPodcast->push_back(new peePodcast(podCast, 100, 1));
		}
	}

	free(line);
	fclose(stream);

	return retPodcast;

}


/*
 *
 * Dont forget to free(fileBuf); !!!
 */
char * toolsGetHtml(char *url) {
	int ret = NULL;
	//char *url = "http://api.deezer.com/user/92847721/albums&output=xml";   	/* Pointer to the url you want */
	char *fileBuf; /* Pointer to downloaded data */

	ret = http_fetch(url, &fileBuf); /* Downloads page */
	if (ret == -1) /* All HTTP Fetcher functions return */
	{
		http_perror("http_fetcha"); /*	-1 on error. */
		return NULL;
	} else {
		//	printf("Page successfully downloaded. (%s)\n", url);
		//	printf("Page successfully downloaded. (%s)\n", fileBuf);

	}

	/*
	 * Now we have the page downloaded and stored in fileBuf, we could save it
	 *	to disk, print it out, etc.  Notice that http_fetch() is the primary
	 *	function of the library, and for a barebones request is the only
	 *	function you need (in conjunction with http_perror).  With HTTP Fetcher
	 *	you can also set User Agent, referer, and timeout values, parse
	 *	filenames, etc... see the header file (http_fetcher.h) for function
	 *	usage/information.
	 */

	return fileBuf;
}

/*
 *  Sound Item management
 *
 *
 */

vector<peeAlbum*>* toolsGetUserAlbums(uint32_t userId) {
	vector<peeAlbum*>* retAlbum = new std::vector<peeAlbum*>;

	printf("user id %u\n",userId);

	toolsGetUserAlbumsIndex(userId, 0, retAlbum);
	toolsGetUserAlbumsIndex(userId, 25, retAlbum);
	toolsGetUserAlbumsIndex(userId, 50, retAlbum);
	toolsGetUserAlbumsIndex(userId, 75, retAlbum);
	toolsGetUserAlbumsIndex(userId, 100, retAlbum);
	toolsGetUserAlbumsIndex(userId, 125, retAlbum);
	toolsGetUserAlbumsIndex(userId, 150, retAlbum);

	return retAlbum;
}

vector<peeAlbum*>* toolsGetUserAlbumsIndex(uint32_t userId, int index, vector<peeAlbum*>* retAlbum) {
	XMLDocument xmlDoc;
	char url[1024];
	char *fileBuf;

	sprintf(url, "http://api.deezer.com/user/%u/albums&output=xml&index=%i", userId, index);
	fileBuf = toolsGetHtml(url);

	xmlDoc.Parse(fileBuf, strlen(fileBuf));
	free(fileBuf);

	//vector<peeAlbum*>* retAlbum =new std::vector<peeAlbum*>;

	if (!xmlDoc.FirstChildElement())
		return retAlbum;

	XMLNode* AlbumNode = xmlDoc.FirstChildElement()->FirstChildElement("data")->FirstChildElement("album");

	while (AlbumNode != NULL) {
		const char* id;
		const char* artisteName;
		const char* albumName;
		const char* coverHtmplPath;

		id = AlbumNode->FirstChildElement("id")->FirstChild()->Value();
		if (AlbumNode->FirstChildElement("album") > 0) {
			id = AlbumNode->FirstChildElement("album")->FirstChildElement("id")->FirstChild()->Value();
		}

		artisteName = AlbumNode->FirstChildElement("artist")->FirstChildElement("name")->FirstChild()->Value();
		albumName = AlbumNode->FirstChildElement("title")->FirstChild()->Value();
		coverHtmplPath = AlbumNode->FirstChildElement("cover_medium")->FirstChild()->Value();

		retAlbum->push_back(new peeAlbum(id, artisteName, albumName, coverHtmplPath));

		AlbumNode = AlbumNode->NextSibling();
	}

	return retAlbum;
}

vector<peeTrack*>* toolsGetUserAlbumTracks(peeAlbum* pAlbum) {
	XMLDocument xmlDoc;
	char url[1024];
	char *fileBuf;
	uint32_t albumId;

	sprintf(url, "http://api.deezer.com/album/%s&output=xml", pAlbum->_id);
	fileBuf = toolsGetHtml(url);

	vector<peeTrack*>* retAlbum = new std::vector<peeTrack*>;

	xmlDoc.Parse(fileBuf, strlen(fileBuf));
	free(fileBuf);

	if (xmlDoc.FirstChildElement()->FirstChildElement("tracks") == NULL)
		return retAlbum;

	XMLNode* tracksNode = xmlDoc.FirstChildElement()->FirstChildElement("tracks")->FirstChildElement("data")->FirstChildElement("track");

	XMLNode* g1 = xmlDoc.FirstChildElement()->FirstChildElement("genres")->FirstChildElement("data")->FirstChildElement("genre");
	if (g1) {
		pAlbum->setGenre(g1->FirstChildElement("name")->FirstChild()->Value());
	} else {
		pAlbum->setGenre("");
	}

	while (tracksNode != NULL) {
		const char* id;
		const char* title;
		const char* artist;
		const char* album;

		int length;
		int pos = 0;

		id = tracksNode->FirstChildElement("id")->FirstChild()->Value();
		title = tracksNode->FirstChildElement("title")->FirstChild()->Value();
		length = atoi(tracksNode->FirstChildElement("duration")->FirstChild()->Value());
		//pos=atoi(tracksNode->FirstChildElement("track_position")->FirstChild()->Value());
		artist = tracksNode->FirstChildElement("artist")->FirstChildElement("name")->FirstChild()->Value();
		//album=tracksNode->FirstChildElement("album")->FirstChildElement("title")->FirstChild()->Value();
		album = pAlbum->_albumName;
		retAlbum->push_back(new peeTrack(id, title, length, pAlbum, album, artist, pos));

		tracksNode = tracksNode->NextSibling();
	}

	return retAlbum;
}




std::vector<peePlaylist*>* toolsGetUserPlaylistsIndex(uint32_t userId, int index, vector<peePlaylist*>* retPlaylist) {
	XMLDocument xmlDoc;
	char url[1024];
	char *fileBuf;

	sprintf(url, "http://api.deezer.com/user/%u/playlists&output=xml&index=%i", userId, index);
	fileBuf = toolsGetHtml(url);

	xmlDoc.Parse(fileBuf, strlen(fileBuf));
	free(fileBuf);

	//vector<peePlaylist*>* retPlaylist = new std::vector<peePlaylist*>;

	if (!xmlDoc.FirstChildElement("root"))
		return retPlaylist;

	XMLNode* AlbumNode = xmlDoc.FirstChildElement("root")->FirstChildElement("data")->FirstChildElement("playlist");

	while (AlbumNode != NULL) {
		const char* id;
		const char* name;
		const char* coverHtmplPath;

		id = AlbumNode->FirstChildElement("id")->FirstChild()->Value();
		name = AlbumNode->FirstChildElement("title")->FirstChild()->Value();
		coverHtmplPath = AlbumNode->FirstChildElement("picture_big")->FirstChild()->Value();

		retPlaylist->push_back(new peePlaylist(id, name, coverHtmplPath));

		AlbumNode = AlbumNode->NextSibling();
	}

	return retPlaylist;
}

std::vector<peePlaylist*>* toolsGetUserPlaylists(uint32_t userId) {
	vector<peePlaylist*>* retPlaylist = new std::vector<peePlaylist*>;

	toolsGetUserPlaylistsIndex(userId,  0,retPlaylist);
	toolsGetUserPlaylistsIndex(userId, 25,retPlaylist);
	toolsGetUserPlaylistsIndex(userId, 50,retPlaylist);
	toolsGetUserPlaylistsIndex(userId, 75,retPlaylist);

	return retPlaylist;
}

vector<peeTrack*>* toolsGetUserPlaylistTracks(peePlaylist* pPlaylist) {
	XMLDocument xmlDoc;
	char url[1024];
	char *fileBuf;
	uint32_t albumId;

	sprintf(url, "http://api.deezer.com/playlist/%s&output=xml", pPlaylist->_id);
	fileBuf = toolsGetHtml(url);

	xmlDoc.Parse(fileBuf, strlen(fileBuf));
	free(fileBuf);
	XMLNode* tracksNode = xmlDoc.FirstChildElement()->FirstChildElement("tracks")->FirstChildElement("data")->FirstChildElement("track");

	vector<peeTrack*>* retAlbum = new std::vector<peeTrack*>;

	while (tracksNode != NULL) {
		const char* id;
		const char* albumid;
		const char* albumCover;
		const char* title;
		const char* artist;
		const char* album;
		int pos = 0;
		int length;

		id = tracksNode->FirstChildElement("id")->FirstChild()->Value();
		title = tracksNode->FirstChildElement("title")->FirstChild()->Value();
		length = atoi(tracksNode->FirstChildElement("duration")->FirstChild()->Value());

		artist = tracksNode->FirstChildElement("artist")->FirstChildElement("name")->FirstChild()->Value();

		album = tracksNode->FirstChildElement("album")->FirstChildElement("title")->FirstChild()->Value();
		albumid = tracksNode->FirstChildElement("album")->FirstChildElement("id")->FirstChild()->Value();
		albumCover = tracksNode->FirstChildElement("album")->FirstChildElement("cover_medium")->FirstChild()->Value();
		peeTrack* pTrack = new peeTrack(id, title, length, pPlaylist, album, artist, pos);
		peeAlbum* pAlbum = new peeAlbum(albumid, artist, album, albumCover);
		pAlbum->downloadPicture();
		pTrack->_pAlbum = pAlbum;
		retAlbum->push_back(pTrack);

		tracksNode = tracksNode->NextSibling();
	}

	return retAlbum;
}

void toolsUpdateUserPodcastTracks(vector<peePodcastTrack*>* podcastList, peePodcast* pParent, char* htmlSource) {
	XMLDocument xmlDoc;
	char *fileBuf = NULL;
	fileBuf = toolsGetHtml(htmlSource);
	if (fileBuf == NULL)
		return;

	xmlDoc.Parse(fileBuf, strlen(fileBuf));
	free(fileBuf);
	XMLNode* podcastNode;
	podcastNode = xmlDoc.FirstChildElement("rss");
	if (!podcastNode)
		return;

	podcastNode = podcastNode->FirstChildElement("channel");

	pParent->setTitle(podcastNode->FirstChildElement("title")->FirstChild()->Value());
	pParent->setImage(podcastNode->FirstChildElement("image")->FirstChildElement("url")->FirstChild()->Value());

	podcastNode = podcastNode->FirstChildElement("item");

	char szCmd[512];
	sprintf(szCmd, "mkdir -p \"%spodcast/%s\"", DOWNLOAD_ROOT_DIR, pParent->_directory);
	system(szCmd);

	while (podcastNode != NULL) {
		tm tsDuration;
		tm date;
		int size;
		const char* title;
		const char* htmlMp3;
		const char* duration;
		const char* pubDate;

		//deprecated, to be updated with the new xml
		title = podcastNode->FirstChildElement("title")->FirstChild()->Value();
		htmlMp3 = podcastNode->FirstChildElement("enclosure")->Attribute("url");
		size = atoi(podcastNode->FirstChildElement("enclosure")->Attribute("length"));
		duration = podcastNode->FirstChildElement("itunes:duration")->FirstChild()->Value();
		pubDate = podcastNode->FirstChildElement("pubDate")->FirstChild()->Value();

		strptime(duration, "%H:%M:%S", &tsDuration);
		strptime(pubDate, "%a, %d %b %Y %H:%M:%S", &date);

		if (((tsDuration.tm_hour * 60) + tsDuration.tm_min) > pParent->_minLength) {
			if (pParent->GetTrackByTitle(title) == NULL) {
				podcastList->push_back(new peePodcastTrack(pParent, &date, title, htmlMp3, size));
			}
		}
		podcastNode = podcastNode->NextSibling();
	}
	return;
}

void toolsPrintAlbums(vector<peeAlbum*>* pAlbum) {
	printf("===== ALBUMS =====\r\n");
	for (vector<peeAlbum*>::iterator it = pAlbum->begin(); it != pAlbum->end(); it++) {
		(*it)->print();
	}
}

void toolsPrintPlaylists(vector<peePlaylist*>* pPlaylist) {
	printf("===== PLAYLISTS =====\r\n");
	for (vector<peePlaylist*>::iterator it = pPlaylist->begin(); it != pPlaylist->end(); it++) {
		(*it)->print();
	}
}

void toolsPrintPodcasts(vector<peePodcast*>* pPodcasts) {
	printf("===== PODCASTS =====\r\n");
	for (vector<peePodcast*>::iterator it = pPodcasts->begin(); it != pPodcasts->end(); it++) {
		(*it)->print();
	}
}

/*
 *
 * IHM
 *
 */

int toolsGetNext(stAppContext* pContext) {

	char c = getchar();

	if (c == '\n') {
		// skip final end of line char
		return 0;
	}

	switch (c) {
	case 'Q':
		return 1;
		break;

	default:
		return 0;
		break;
	}
	return 0;
}

int toolsCleanUTF8(char* szString) {
	int len = strlen(szString);
	int dst = 0;
	for (int i = 0; i < len; i++) {
		if ((szString[i] & 0xF0) == 0xC0) {
			i++;

			if ((szString[i] == 0xa8) || (szString[i] == 0xa9) || (szString[i] == 0xaa) || (szString[i] == 0xab)) //223 -> é  //232 -> è
					{
				szString[dst] = 'e';
			} else if ((szString[i] == 0xa7)) //221 -> ç
			{
				szString[dst] = 'c';
			} else if ((szString[i] == 0xa0) || (szString[i] == 0xa1) || (szString[i] == 0xa2)) {
				szString[dst] = 'a';
			} else if ((szString[i] == 0xb2) || (szString[i] == 0xb3) || (szString[i] == 0xb4)) {
				szString[dst] = 'o';
			} else if ((szString[i] == 0xaf) || (szString[i] == 0xad)) {
				szString[dst] = 'i';
			} else if ((szString[i] == 0xbb) || (szString[i] == 0x80) || (szString[i] == 0xae)) {
				szString[dst] = ' ';
			} else {
				szString[dst] = ' ';
				printf("Missed! 0x%x : %s\n", szString[i], szString);
			}
		} else if ((szString[i] & 0xF0) == 0xE0) {
			i += 3;
			szString[dst] = ' ';
		} else if ((szString[i] & 0xF0) == 0xF0) {
			i += 4;
			szString[dst] = ' ';
		}

		else if (isalnum(szString[i]) || szString[i] == ' ' || szString[i] == '/' || szString[i] == '.') {
			szString[dst] = szString[i];
		} else {
			szString[dst] = ' ';
		}

		dst++;
	}
	szString[dst] = 0;

	while (szString[strlen(szString) - 1] == ' ')
		szString[strlen(szString) - 1] = 0;

	dst = 0;
	for (unsigned int ii = 0; ii < strlen(szString) - 1; ii++) {
		if (szString[ii] == ' ' && szString[ii + 1] == '/') {
			ii++;
		}
		szString[dst] = szString[ii];
		dst++;
	}

	return 0;
}

void toolsDownloaderTracks(vector<peeAlbum*>* pAlbum) {
	for (vector<peeAlbum*>::iterator it = pAlbum->begin(); it != pAlbum->end(); it++) {
		printf("Album: %s %s\n", (*it)->_artisteName, (*it)->_albumName);

		(*it)->fetchTracks();

		if ((*it)->GetNbrTracksDownloaded() < (*it)->GetNbrTracks()) {
			deezerPostCommand(DEEZER_CMD_LOAD_ALBUM, (*it), NULL);
			do {
				sleep(1);
			} while (deezerIsPlaying());
		} else {
			printf("   fully downloaded\n");
			sleep(1);
		}

	}
}

void toolsDownloaderPlaylist(vector<peePlaylist*>* pPlaylist) {
	for (vector<peePlaylist*>::iterator it = pPlaylist->begin(); it != pPlaylist->end(); it++) {
		printf("Playlist: %s \n", (*it)->_name);

		(*it)->fetchTracks();


		 if ((*it)->GetNbrTracksDownloaded() < (*it)->GetNbrTracks()) {
			deezerPostCommand(DEEZER_CMD_LOAD_PLAYLIST, (*it), NULL);
			do {
				sleep(1);
			} while (deezerIsPlaying());
		} else {
			printf("   fully downloaded\n");
			sleep(1);
		}

	}
}
