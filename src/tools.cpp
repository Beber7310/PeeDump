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
#include <time.h>

#include <semaphore.h>
#include <pthread.h>

#include "tinyxml2.h"
#include "http_fetcher.h"
#include "peeAlbum.h"
#include "peePlaylist.h"
#include "deezer.h"
#include "tools.h"
#include "main.h"
#include "downloader.h"

using namespace tinyxml2;
using namespace std;




uint32_t  toolsGetUser()
{
	uint32_t idUser=0;

	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	stream = fopen("/home/pi/projects/data.conf", "r");
	if (stream == NULL)
	{
		printf("Unable to open /home/pi/projects/data.conf\n");
		exit(EXIT_FAILURE);
	}
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
	char* podCast;
	size_t len = 0;
	ssize_t read;
	std::vector<peePodcast*>*retPodcast =new std::vector<peePodcast*>();


	stream = fopen("/home/pi/projects/data.conf", "r");
	if (stream == NULL)
	{
			printf("Unable to open /home/pi/projects/data.conf\n");
			exit(EXIT_FAILURE);
		}
	while ((read = getline(&line, &len, stream)) != -1) {
		if(strncmp("lpodcast:",line,strlen("lpodcast:"))==0)
		{
			podCast=&line[strlen("lpodcast:")];
			podCast[strlen(podCast)-1]=0;// remove last char as it is a \n

			retPodcast->push_back(new peePodcast(podCast,100,10));
		}
		if(strncmp("spodcast:",line,strlen("spodcast:"))==0)
		{
			podCast=&line[strlen("spodcast:")];
			podCast[strlen(podCast)-1]=0;// remove last char as it is a \n

			retPodcast->push_back(new peePodcast(podCast,100,1));
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

	stream = fopen("/home/pi/projects/data.conf", "r");
	if (stream == NULL)
	{
			printf("Unable to open /home/pi/projects/data.conf\n");
			exit(EXIT_FAILURE);
		}

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
	int ret=NULL;
	//char *url = "http://api.deezer.com/user/92847721/albums&output=xml";   	/* Pointer to the url you want */
	char *fileBuf;						/* Pointer to downloaded data */

	ret = http_fetch(url, &fileBuf);	/* Downloads page */
	if(ret == -1)						/* All HTTP Fetcher functions return */
	{
		http_perror("http_fetcha");		/*	-1 on error. */
		return NULL;
	}
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
	XMLNode* AlbumNode = xmlDoc.FirstChildElement( )->FirstChildElement( "data" )->FirstChildElement( "album");

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

void toolsUpdateUserPodcastTracks(vector<peePodcastTrack*>* podcastList,peePodcast* pParent,char* htmlSource)
{
	XMLDocument xmlDoc;
	char *fileBuf=NULL;
	fileBuf = toolsGetHtml(htmlSource);
	if(fileBuf==NULL)
		return;

	xmlDoc.Parse( fileBuf, strlen(fileBuf));
	free(fileBuf);
	XMLNode* podcastNode;
	podcastNode = xmlDoc.FirstChildElement( "rss" );
	podcastNode = podcastNode->FirstChildElement( "channel" );

	pParent->setTitle(podcastNode->FirstChildElement( "title")->FirstChild()->Value());
	pParent->setImage(podcastNode->FirstChildElement( "image")->FirstChildElement( "url")->FirstChild()->Value());

	podcastNode = podcastNode->FirstChildElement( "item");

	char szCmd[512];
	sprintf(szCmd,"mkdir -p \"%spodcast/%s\"",DOWNLOAD_ROOT_DIR,pParent->_directory);
	system(szCmd);

	while(podcastNode!=NULL)
	{
		tm tsDuration;
		tm	date;
		int size;
		const char*	title;
		const char*	htmlMp3;
		const char* duration;
		const char* pubDate;

		//deprecated, to be updated with the new xml
		title=podcastNode->FirstChildElement("title")->FirstChild()->Value();
		htmlMp3=podcastNode->FirstChildElement( "enclosure" )->Attribute( "url");
		size=atoi(podcastNode->FirstChildElement( "enclosure" )->Attribute( "length"));
		duration=podcastNode->FirstChildElement( "itunes:duration" )->FirstChild()->Value();
		pubDate=podcastNode->FirstChildElement( "pubDate" )->FirstChild()->Value();

		strptime(duration, "%H:%M:%S", &tsDuration);
		strptime(pubDate, "%a, %d %b %Y %H:%M:%S", &date);

		if(((tsDuration.tm_hour*60)+tsDuration.tm_min)>pParent->_minLength)
		{
			if(pParent->GetTrackByTitle(title)==NULL)
			{
				podcastList->push_back(new peePodcastTrack(pParent,&date,title,htmlMp3,size));
			}
		}

		podcastNode=podcastNode->NextSibling();
	}
	return;
}

void toolsPrintAlbums(vector<peeAlbum*>* pAlbum)
{
	printf("===== ALBUMS =====\r\n");
	for (vector<peeAlbum*>::iterator it = pAlbum->begin(); it != pAlbum->end(); it++)
	{
		(*it)->print();
	}
}

void toolsPrintPlaylists(vector<peePlaylist*>* pPlaylist)
{
	printf("===== PLAYLISTS =====\r\n");
	for (vector<peePlaylist*>::iterator it = pPlaylist->begin(); it != pPlaylist->end(); it++)
	{
		(*it)->print();
	}
}

void toolsPrintPodcasts(vector<peePodcast*>* pPodcasts)
{
	printf("===== PODCASTS =====\r\n");
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

		default:
			return 0;
			break;
	}
	return 0;
}



int toolsCleanUTF8(char* szString)
{
	int len=strlen(szString);
	int dst=0;
	for(int i=0;i<len;i++)
	{
		if(szString[i]>0xC0)
		{
			i++;

			if((szString[i]==0xa8)||(szString[i]==0xa9)||(szString[i]==0xaa)||(szString[i]==0xab) ) //223 -> é  //232 -> è
			{
				szString[dst]='e';
			}
			else if((szString[i]==0xa7)) //221 -> ç
			{
				szString[dst]='c';
			}
			else if((szString[i]==0xa0)||(szString[i]==0xa1)||(szString[i]==0xa2))
			{
				szString[dst]='a';
			}
			else if((szString[i]==0xb2)||(szString[i]==0xb3)||(szString[i]==0xb4))
			{
				szString[dst]='o';
			}
			else if((szString[i]==0xaf))
			{
				szString[dst]='i';
			}
			else if((szString[i]==0xbb)||(szString[i]==0x80)||(szString[i]==0xae))
			{
				szString[dst]=' ';
			}
			else
			{
				i++;
				//printf("Missed! 0x%x : %s\n",szString[i],szString);
			}
		}
		else if(isalnum(szString[i]) || szString[i]==' '|| szString[i]=='/'|| szString[i]=='.')
		{
			szString[dst]=szString[i];
		}
		else
		{
			szString[dst]=' ';
		}


		dst++;
	}
	szString[dst]=0;
	return 0;
}

