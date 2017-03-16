/*
 * tools.c
 *
 *  Created on: 10 mars 2017
 *      Author: Bertrand
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>

#include "tinyxml2.h"
#include "http_fetcher.h"
#include "peeAlbum.h"
#include "peePlaylist.h"
#include "deezer.h"
#include "tools.h"
#include "main.h"

using namespace tinyxml2;
using namespace std;

uint32_t  toolsGetUser()
{
	uint32_t idUser=0;

	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	stream = fopen("data.conf", "r");
	if (stream == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, stream)) != -1) {
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s", line);
		if(strncmp("user:",line,strlen("user:"))==0)
		{
			idUser=atoi(&line[strlen("user:")]);
		}
	}

	free(line);
	fclose(stream);

	printf("User:  %i\n", idUser);

	return idUser;

}

std::vector<peePodcast*>*  toolsGetPodcast(void)
{
	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	std::vector<peePodcast*>*retPodcast =new std::vector<peePodcast*>();


	stream = fopen("data.conf", "r");
	if (stream == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, stream)) != -1) {
		if(strncmp("podcast:",line,strlen("user:"))==0)
		{
			retAlbum->push_back(new peePodcasts(&line[strlen("podcast:")]));
		}
	}

	free(line);
	fclose(stream);

	return retPodcast;

}


void toolsGetToken(char* pToken)
{

	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	stream = fopen("data.conf", "r");
	if (stream == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, stream)) != -1) {
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s", line);
		if(strncmp("token:",line,strlen("user:"))==0)
		{
			//idUser=atoi(&line[strlen("user:")]);
			pToken=strcpy(pToken,&line[strlen("token:")]);
		}
	}
	pToken[strlen(pToken)-1]=0;// remove last char as it is a \n

	free(line);
	fclose(stream);

	printf("Token: <%s>\n", pToken);

	return;
}


/*
 *
 * Dont forget to free(fileBuf); !!!
 */
char * toolsGetHtml(char *url)
{
	int ret;
	//char *url = "http://api.deezer.com/user/92847721/albums&output=xml";   	/* Pointer to the url you want */
	char *fileBuf;						/* Pointer to downloaded data */

	ret = http_fetch(url, &fileBuf);	/* Downloads page */
	if(ret == -1)						/* All HTTP Fetcher functions return */
		http_perror("http_fetch");		/*	-1 on error. */
	else
	{
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
vector<peeAlbum*>* toolsGetUserAlbums(uint32_t userId)
{
	XMLDocument xmlDoc;
	char url [1024];
	char *fileBuf;

	sprintf(url,"http://api.deezer.com/user/%i/albums&output=xml",userId);
	fileBuf = toolsGetHtml(url);

	xmlDoc.Parse( fileBuf, strlen(fileBuf));
	free(fileBuf);
	XMLNode* AlbumNode = xmlDoc.FirstChildElement( "root" )->FirstChildElement( "data" )->FirstChildElement( "album");

	vector<peeAlbum*>* retAlbum =new std::vector<peeAlbum*>;

	while(AlbumNode!=NULL)
	{
		const char* id;
		const char* artisteName;
		const char* albumName;
		const char* coverHtmplPath;

		id=AlbumNode->FirstChildElement("id")->FirstChild()->Value();

		artisteName=AlbumNode->FirstChildElement("artist")->FirstChildElement("name")->FirstChild()->Value();
		albumName=AlbumNode->FirstChildElement("title")->FirstChild()->Value();
		coverHtmplPath=AlbumNode->FirstChildElement("cover_big")->FirstChild()->Value();


		retAlbum->push_back(new peeAlbum(id,artisteName,albumName,coverHtmplPath));

		AlbumNode=AlbumNode->NextSibling();
	}

	return retAlbum;
}

std::vector<peePlaylist*>* toolsGetUserPlaylists(uint32_t userId)
{
	XMLDocument xmlDoc;
	char url [1024];
	char *fileBuf;

	sprintf(url,"http://api.deezer.com/user/%i/playlists&output=xml",userId);
	fileBuf = toolsGetHtml(url);

	xmlDoc.Parse( fileBuf, strlen(fileBuf));
	free(fileBuf);

	XMLNode* AlbumNode = xmlDoc.FirstChildElement( "root" )->FirstChildElement( "data" )->FirstChildElement( "playlist");

	vector<peePlaylist*>* retPlaylist =new std::vector<peePlaylist*>;

	while(AlbumNode!=NULL)
	{
		const char* id;
		const char* name;
		const char* coverHtmplPath;

		id=AlbumNode->FirstChildElement("id")->FirstChild()->Value();
		name=AlbumNode->FirstChildElement("title")->FirstChild()->Value();
		coverHtmplPath=AlbumNode->FirstChildElement("picture_big")->FirstChild()->Value();

		retPlaylist->push_back(new peePlaylist(id,name,coverHtmplPath));

		AlbumNode=AlbumNode->NextSibling();
	}

	return retPlaylist;
}

vector<peePodcast*>* toolsGetUserPodcasts(uint32_t userId)
{
	XMLDocument xmlDoc;
	char url [1024];
	char *fileBuf;

	sprintf(url,"http://api.deezer.com/user/%i/podcasts&output=xml",userId);
	fileBuf = toolsGetHtml(url);

	xmlDoc.Parse( fileBuf, strlen(fileBuf));
	free(fileBuf);
	XMLNode* podcastNode = xmlDoc.FirstChildElement( "root" )->FirstChildElement( "data" )->FirstChildElement( "podcast");

	vector<peePodcast*>* retPodcast =new std::vector<peePodcast*>;

	while(podcastNode!=NULL)
	{
		const char* id;
		const char* title;

		const char* coverHtmplPath;

		id=podcastNode->FirstChildElement("id")->FirstChild()->Value();

		title=podcastNode->FirstChildElement("title")->FirstChild()->Value();
		coverHtmplPath=podcastNode->FirstChildElement("picture")->FirstChild()->Value();


		retPodcast->push_back(new peePodcast(id,title,coverHtmplPath));

		podcastNode=podcastNode->NextSibling();
	}

	return retPodcast;
}

vector<peePodcastTrack*>* toolsGetUserPodcastTracks(peePodcast* pParent,char* htmlSource)
{
	XMLDocument xmlDoc;
	char *fileBuf;

	fileBuf = toolsGetHtml(htmlSource);

	xmlDoc.Parse( fileBuf, strlen(fileBuf));
	free(fileBuf);
	XMLNode* podcastNode = xmlDoc.FirstChildElement( "root" )->FirstChildElement( "data" )->FirstChildElement( "podcast");

	vector<peePodcastTrack*>* retPodcast =new std::vector<peePodcastTrack*>;

	while(podcastNode!=NULL)
	{
		//deprecated, to be updated with the new xml
		const char* id;
		const char* title;
		const char* coverHtmplPath;


		// value to be keeped
		time_t	date;
		char*	title;
		char* htmlPath;

		//deprecated, to be updated with the new xml
		id=podcastNode->FirstChildElement("id")->FirstChild()->Value();
		title=podcastNode->FirstChildElement("title")->FirstChild()->Value();
		coverHtmplPath=podcastNode->FirstChildElement("picture")->FirstChild()->Value();


		retPodcast->push_back(new podcasttrack(pParent,date,title,htmlPath));

		podcastNode=podcastNode->NextSibling();
	}

	return retPodcast;
}


void toolsPrintAlbums(vector<peeAlbum*>* pAlbum)
{
	for (vector<peeAlbum*>::iterator it = pAlbum->begin(); it != pAlbum->end(); it++)
	{
		(*it)->print();
	}
}

void toolsPrintPlaylists(vector<peePlaylist*>* pPlaylist)
{
	for (vector<peePlaylist*>::iterator it = pPlaylist->begin(); it != pPlaylist->end(); it++)
	{
		(*it)->print();
	}
}

void toolsPrintPodcasts(vector<peePodcast*>* pPodcasts)
{
	for (vector<peePodcast*>::iterator it = pPodcasts->begin(); it != pPodcasts->end(); it++)
	{
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

		case 'S':
			deezerPostCommand(DEEZER_CMD_START,0);
			break;
		case 'R':
			deezerPostCommand(DEEZER_CMD_RESUME,0);
			break;
		case 's':
			deezerPostCommand(DEEZER_CMD_STOP,0);
			break;
		case 'p':
			toolsPrintAlbums(pContext->Albums);
			break;
		case '+':
			deezerPostCommand(DEEZER_CMD_NEXT,0);
			break;
		case '-':
			deezerPostCommand(DEEZER_CMD_PREV,0);
			break;
		case '1':
			deezerPostCommand(DEEZER_CMD_LOAD_ALBUM,pContext->Albums->at(0)->_id);
			break;
		case '2':
			deezerPostCommand(DEEZER_CMD_LOAD_ALBUM,pContext->Albums->at(1)->_id);
			break;
		case '3':
			deezerPostCommand(DEEZER_CMD_LOAD_ALBUM,pContext->Albums->at(2)->_id);
			break;
		default:
			return 0;
			break;
	}
	return 0;
}


