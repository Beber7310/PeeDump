/*
 * tools.c
 *
 *  Created on: 10 mars 2017
 *      Author: Bertrand
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <list>

#include "tinyxml2.h"
#include "http_fetcher.h"
#include "peeAlbum.h"
#include "peePlaylist.h"
#include "deezer.h"

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
list<peeAlbum*>* toolsGetUserAlbums(uint32_t userId)
		{
	XMLDocument xmlDoc;
	char url [1024];
	char *fileBuf;

	sprintf(url,"http://api.deezer.com/user/%i/albums&output=xml",userId);
	fileBuf = toolsGetHtml(url);

	xmlDoc.Parse( fileBuf, strlen(fileBuf));
	free(fileBuf);
	XMLNode* AlbumNode = xmlDoc.FirstChildElement( "root" )->FirstChildElement( "data" )->FirstChildElement( "album");

	list<peeAlbum*>* retAlbum =new std::list<peeAlbum*>;

	while(AlbumNode!=NULL)
	{
		uint32_t id;
		const char* artisteName;
		const char* albumName;
		const char* coverHtmplPath;

		id=atoi(AlbumNode->FirstChildElement("id")->FirstChild()->Value());

		artisteName=AlbumNode->FirstChildElement("artist")->FirstChildElement("name")->FirstChild()->Value();
		albumName=AlbumNode->FirstChildElement("title")->FirstChild()->Value();
		coverHtmplPath=AlbumNode->FirstChildElement("cover_big")->FirstChild()->Value();


		retAlbum->push_back(new peeAlbum(id,artisteName,albumName,coverHtmplPath));

		/*
		printf( "album name: %s\n", AlbumNode->FirstChildElement("title")->FirstChild()->Value() );
		printf( "     %s\n", AlbumNode->FirstChildElement("artist")->FirstChildElement("name")->FirstChild()->Value() );
		printf( "     %s\n", AlbumNode->FirstChildElement("id")->FirstChild()->Value() );
		 */




		AlbumNode=AlbumNode->NextSibling();
	}

	return retAlbum;
		}

std::list<peePlaylist*>* toolsGetUserPlaylists(uint32_t userId)
														{
	XMLDocument xmlDoc;
	char url [1024];
	char *fileBuf;

	sprintf(url,"http://api.deezer.com/user/%i/playlists&output=xml",userId);
	fileBuf = toolsGetHtml(url);

	xmlDoc.Parse( fileBuf, strlen(fileBuf));
	free(fileBuf);

	XMLNode* AlbumNode = xmlDoc.FirstChildElement( "root" )->FirstChildElement( "data" )->FirstChildElement( "playlist");

	list<peePlaylist*>* retPlaylist =new std::list<peePlaylist*>;

	while(AlbumNode!=NULL)
	{
		uint32_t id;
		const char* name;
		id=atoi(AlbumNode->FirstChildElement("id")->FirstChild()->Value());
		name=AlbumNode->FirstChildElement("title")->FirstChild()->Value();
		retPlaylist->push_back(new peePlaylist(id,name));

		/*
		printf( "playlist name: %s\n", AlbumNode->FirstChildElement("title")->FirstChild()->Value() );
		printf( "     %s\n", AlbumNode->FirstChildElement("id")->FirstChild()->Value() );
		 */


		AlbumNode=AlbumNode->NextSibling();
	}

	return retPlaylist;
														}

void toolsPrintAlbums(list<peeAlbum*>* pAlbum)
{
	for (list<peeAlbum*>::iterator it = pAlbum->begin(); it != pAlbum->end(); it++)
	{
		(*it)->print();
	}
}

void toolsPrintPlaylists(list<peePlaylist*>* pPlaylist)
{
	for (list<peePlaylist*>::iterator it = pPlaylist->begin(); it != pPlaylist->end(); it++)
	{
		(*it)->print();
	}
}

/*
 *
 * IHM
 *
 */

int toolsGetNext() {

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
			deezerPostCommand(DEEZER_CMD_START,NULL);
			break;
		case 'R':
			deezerPostCommand(DEEZER_CMD_RESUME,NULL);
			break;
		case '+':
			deezerPostCommand(DEEZER_CMD_NEXT,NULL);
			break;
		case '1':
			deezerPostCommand(DEEZER_CMD_LOAD_ALBUM,303936);
			break;
		default:
			return 0;
			break;
	}
	return 0;

	/*switch (c) {
		case 'S':
			app_playback_start_or_stop();
			break;

		case 'P':
			app_playback_pause_or_resume();
			break;

		case '+':
			app_playback_next();
			break;

		case '-':
			app_playback_previous();
			break;

		case 'R':
			app_playback_toogle_repeat();
			break;

		case '?':
			app_playback_toogle_random();
			break;

		case 'Q':
			is_shutting_down = true;
			app_shutdown();
			break;
		case '1':
			app_change_content("dzmedia:///album/607845");
			app_load_content();
			break;
		case '2':
			app_change_content("dzmedia:///playlist/1363560485");
			app_load_content();
			break;
		case '3':
			app_change_content("dzradio:///user-743548285");
			app_load_content();
			break;
		case '4':
			app_change_content("dzmedia:///track/10287076");
			app_load_content();
			break;

		default:
			log(" - Command [%c] not recognised -\n",c);
			app_commands_display();
			break;
	}*/

}
