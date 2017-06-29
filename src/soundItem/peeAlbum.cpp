/*
 * peeAlbum.cpp
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#include "configuration.h"

#include "peeAlbum.h"
#include "peeBase.h"
#include "tools.h"
#include"downloader.h"

using namespace std;

peeAlbum::peeAlbum() {
	// TODO Auto-generated constructor stub
	_id=NULL;
	_artisteName=NULL;
	_albumName=NULL;
	_coverHtmplPath=NULL;
	_tracks=NULL;
	_currentTrack=0;
	_nbrTracksDownloaded=0;
	_localPathCover=NULL;
	_localDir=NULL;
}

peeAlbum::~peeAlbum() {
	// TODO Auto-generated destructor stub
}

peeAlbum::peeAlbum(const char* id,const char* artisteName,const char* albumName,const char* coverHtmplPath){
	// TODO Auto-generated constructor stub
	_id=(char*)malloc(strlen(id)+1);;
	_artisteName=(char*)malloc(strlen(artisteName)+1);
	_albumName=(char*)malloc(strlen(albumName)+1);
	_coverHtmplPath=(char*)malloc(strlen(coverHtmplPath)+1);

	strcpy(_id,id);
	strcpy(_albumName,albumName);
	strcpy(_artisteName,artisteName);
	strcpy(_coverHtmplPath,coverHtmplPath);

	_localDir=(char*)malloc(strlen(DOWNLOAD_ROOT_DIR)+strlen(_artisteName)+strlen(_albumName)+20);
	sprintf(_localDir,"%s/mp3/%s/%s",DOWNLOAD_ROOT_DIR,_artisteName,_albumName);
	toolsCleanUTF8(_localDir);

	_localPathCover=(char*)malloc(strlen(_localDir)+strlen("cover.jpeg")+20);
	sprintf(_localPathCover,"%s/cover.jpeg",_localDir);

	_currentTrack=0;
	_tracks=NULL;
	_nbrTracksDownloaded=0;
	downloadPicture();
}

void peeAlbum::fetchTracks() {
	if(_tracks==NULL)
	{
		_tracks=toolsGetUserAlbumTracks(this);
		ComputeNbrTracksDownloaded();
	}
}

void peeAlbum::print() {

	cout << _artisteName << " " << _albumName << "(" << _id << ")" <<  endl;
	if(_tracks!=NULL)
	{
		for (vector<peeTrack*>::iterator it = _tracks ->begin(); it != _tracks->end(); it++)
		{
			(*it)->print();
		}
	}
}

void peeAlbum::downloadPicture() {
	char szCmd[1024];

	if(!toolsDownloadExist(_localPathCover))
	{
		sprintf(szCmd,"mkdir -p \"%s\"",_localDir);
		system(szCmd);

		sprintf(szCmd,"wget \"%s\" -O \"%s\" -q --limit-rate=100k",_coverHtmplPath,_localPathCover);
		system(szCmd);
		printf("%s\n",szCmd);
	}
}

int peeAlbum::GetNbrTracksDownloaded(){
	return _nbrTracksDownloaded;
}

void peeAlbum::ComputeNbrTracksDownloaded(){
	_nbrTracksDownloaded=0;

	if(_tracks!=NULL)
	{
		for (vector<peeTrack*>::iterator it = _tracks ->begin(); it != _tracks->end(); it++)
		{
			if((*it)->checkDownload())
			{
				_nbrTracksDownloaded++;
			}
			else
			{
				printf("Not downloaded: %s %s\n",(*it)->_szAlbum,(*it)->_szArtist);
			}
		}
	}

}


int peeAlbum::GetNbrTracks(){
	int nbr=0;

	if(_tracks!=NULL)
	{
		nbr=_tracks->size();
	}
	return nbr;
}

int peeAlbum::GetFirstTrack()
{
	_currentTrack =0;
	for(unsigned int ii=_currentTrack;ii<_tracks->size();ii++)
	{

		if(!_tracks->at(ii)->checkDownload())
		{
			return _currentTrack;
		}
		_currentTrack++;
	}
	return _currentTrack;


}

int peeAlbum::GetNextTrack()
{
	for(unsigned int ii=_currentTrack;ii<_tracks->size();ii++)
	{
		_currentTrack++;
		if(!_tracks->at(ii)->checkDownload())
		{
			return _currentTrack;
		}
	}
	return _currentTrack;
}

