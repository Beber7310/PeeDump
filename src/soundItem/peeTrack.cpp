/*
 * peeTrack.cpp
 *
 *  Created on: 12 avr. 2017
 *      Author: Bertrand
 */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <memory>
#include <iostream>

class peePodcastTrack;//used to solve an ugly issue with downloader header

#include <peeTrack.h>
#include "downloader.h"
#include "configuration.h"
#include "tools.h"


using namespace std;

peeTrack::peeTrack(const char* id,const char* title,int length,peeAlbum* pAlbum,const char* szAlbum, const char* szArtist,int pos) {
	// TODO Auto-generated constructor stub
	_id=(char*)malloc(strlen(id)+1);;
	_title=(char*)malloc(strlen(title)+1);
	_szAlbum=(char*)malloc(strlen(szAlbum)+1);
	_szArtist=(char*)malloc(strlen(szArtist)+1);

	strcpy(_id,id);
	strcpy(_title,title);
	strcpy(_szAlbum,szAlbum);
	strcpy(_szArtist,szArtist);

	_length=length;
	_pAlbum=pAlbum;
	_pPlaylist=NULL;
	_Position=pos;

	_localDir=_pAlbum->_localDir;
	_localPath=(char*)malloc(strlen(_localDir)+strlen(_id)+20);
	sprintf(_localPath,"%s/%s.mp3",_localDir,_id);

}

peeTrack::peeTrack(const char* id, const char* title, int length, peePlaylist* pPlaylist, const char* szAlbum, const char* szArtist,int pos) {

	// TODO Auto-generated constructor stub
	_id=(char*)malloc(strlen(id)+1);;
	_title=(char*)malloc(strlen(title)+1);
	_szAlbum=(char*)malloc(strlen(szAlbum)+1);
	_szArtist=(char*)malloc(strlen(szArtist)+1);

	strcpy(_id,id);
	strcpy(_title,title);
	strcpy(_szAlbum,szAlbum);
	strcpy(_szArtist,szArtist);

	_length=length;
	_pPlaylist=pPlaylist;
	_pPlaylist=NULL;
	_Position=pos;

	_localDir=pPlaylist->_localDir;
	_localPath=(char*)malloc(strlen(_localDir)+strlen(_id)+20);
	sprintf(_localPath,"%s/%s.mp3",_localDir,_id);

	_pAlbum=NULL;
}

peeTrack::~peeTrack() {
	// TODO Auto-generated destructor stub
}

void peeTrack::print() {
	/*if(_pAlbum)
		cout << "   " << _title << " " << _pAlbum->_albumName << "(" << _id << ")" <<  endl;*/

	cout << "   " << _title << " " << _szAlbum << "(" << _id << ")" <<  endl;
}

bool peeTrack::checkDownload(void)
{
	bool downloaded=false;

	if(toolsDownloadExist( _localPath))
	{
		int realSize=toolsDownloadFileSize(_localPath);
		if(realSize<512*1024)
		{
			printf("<checkDownload> File too small %s %i\n",_localPath,realSize);
			downloaded=false;
		}
		else
		{
			downloaded=true;
		}
	}
	else
	{
		printf("<checkDownload> File not found %s\n",_localPath);
		downloaded=false;
	}

	return downloaded;
}


