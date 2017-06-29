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
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "configuration.h"
#include "tools.h"
#include "downloader.h"

#include "peePodcastTrack.h"


peePodcastTrack::peePodcastTrack() {
	// TODO Auto-generated constructor stub
	_title=NULL;
	_htmlPath=NULL;
	_localPath=NULL;
	_size=0;
	_downloaded=false;
}

peePodcastTrack::peePodcastTrack(peePodcast *pParent,tm* date, const char* title,const char* htmlPath,int size) {
	// TODO Auto-generated constructor stub
	char szTmpTitle[512];

	memcpy(&_date,date,sizeof(tm));

	_title=(char*)malloc(strlen(title)+1);
	strcpy(_title,title);

	_htmlPath=(char*)malloc(strlen(htmlPath)+1);
	strcpy(_htmlPath,htmlPath);

	_size=size;

	char strtime[128];
	strftime(strtime, sizeof(strtime), "%Y-%m-%d", &_date);
	_localPath=(char*)malloc(strlen(DOWNLOAD_ROOT_DIR)+strlen(pParent->_directory)+strlen(strtime)+strlen(_title)+50);
	_localPath[0]=0;
	sprintf (_localPath,"%s/",DOWNLOAD_ROOT_DIR);
	strcat (_localPath,"podcast/");
	strcat (_localPath,pParent->_directory);
	strcat (_localPath,"/");
	strcat (_localPath,strtime);
	strcat (_localPath,"-");

	strcpy(szTmpTitle,title);
	int len=strlen(title);

	for(int ii=0;ii<len;ii++)
	{
		if(szTmpTitle[ii]=='/')
			szTmpTitle[ii]=' ';
	}


	strcat (_localPath,szTmpTitle);
	strcat (_localPath,".mp3");

	toolsCleanUTF8(_localPath);

	_downloaded = false;

	checkDownload();
	if(!_downloaded)
	{
		toolsDownloadPodcast(this);
	}
}

void peePodcastTrack::checkDownload(void)
{
	_downloaded=false;

	if(toolsDownloadExist( _localPath))
	{
		int realSize=toolsDownloadFileSize(_localPath);
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


}

peePodcastTrack::~peePodcastTrack() {
	// TODO Auto-generated destructor stub
	if(_title)
		free(_title);
	if(_htmlPath)
		free(_htmlPath);
	if(_localPath)
		free(_localPath);
}

