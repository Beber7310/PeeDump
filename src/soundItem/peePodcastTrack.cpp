/*
 * peePo	dcatTrack.cpp
 *
 *  Created on: Mar 16, 2017
 *      Author: dosdab
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "tools.h"
#include "downloader.h"

#include "peePodcastTrack.h"


peePodcastTrack::peePodcastTrack() {
	// TODO Auto-generated constructor stub
}

peePodcastTrack::peePodcastTrack(peePodcast *pParent,tm* date, const char* title,const char* htmlPath,int size) {
	// TODO Auto-generated constructor stub
	memcpy(&_date,date,sizeof(tm));

	_title=(char*)malloc(strlen(title)+1);
	strcpy(_title,title);

	_htmlPath=(char*)malloc(strlen(htmlPath)+1);
	strcpy(_htmlPath,htmlPath);

	_size=size;

	char strtime[128];
	strftime(strtime, sizeof(strtime), "%Y-%m-%d", &_date);
	_localPath=(char*)malloc(strlen(pParent->_title)+strlen(strtime)+strlen(_title)+50);
	_localPath[0]=0;
	strcpy (_localPath,"podcast/");
	strcat (_localPath,pParent->_title);
	strcat (_localPath,"/");
	strcat (_localPath,strtime);
	strcat (_localPath,"-");
	strcat (_localPath,title);
	strcat (_localPath,".mp3");

	_downloaded =false;

	checkDownload();
}


void peePodcastTrack::checkDownload(void)
{
	_downloaded=false;

	if(access( _localPath, F_OK ) != -1 )
	{
		int realSize=fileSize(_localPath);
		if(realSize<(9*(_size/10)))
		{
			_downloaded=false;
			printf("Pod cast %s wrong size expecting %i get %i\n",_localPath,_size,realSize);
		}
		else
		{
			_downloaded=true;
		}
	}
	else
	{
		_downloaded=false;
		printf("Pod cast %s not existing\n",_localPath);
	}

	if(!_downloaded)
	{
		 //DOWNLOAD HERE
		toolsDownloadPodcast(this);
	}
}


peePodcastTrack::~peePodcastTrack() {
	// TODO Auto-generated destructor stub
}

