/*
 * peePodcast.cpp
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>    // std::sort
#include <memory>

#include "peePodcast.h"
#include "peeBase.h"
#include "tools.h"
#include "downloader.h"

using namespace std;

peePodcast::peePodcast() {
	// TODO Auto-generated constructor stub
	_id=NULL;
	_titleUTF8=NULL;
	_directory=NULL;
	_coverHtmplPath=NULL;
	_htmlSource=NULL;

	_maxAge=1000;
	_minLength=10;
	_traks=NULL;
}

peePodcast::peePodcast(char* htmlSource,int age,int time){
	// TODO Auto-generated constructor stub
	_titleUTF8="";
	_directory="";
	_coverHtmplPath="";
	_htmlSource=(char*)malloc(strlen(htmlSource)+1);
	strcpy(_htmlSource,htmlSource);

	_maxAge=age;
	_minLength=time;
	_traks =new std::vector<peePodcastTrack*>;

	//_traks=toolsGetUserPodcastTracks(this,_htmlSource);
	toolsUpdateUserPodcastTracks(_traks,this,_htmlSource);


	// to be removed as used to test update
	if(_traks->size()>0)
		_traks->erase(_traks->cbegin() + 0);
	//updatePodcast();


	printf("Podcast created\n");
}

peePodcast::~peePodcast() {
	// TODO Auto-generated destructor stub
}

void peePodcast::updatePodcast()
{
 	toolsUpdateUserPodcastTracks(_traks,this,_htmlSource);
	std::sort(_traks->begin(), _traks->end(), compare_podcast);


}

unsigned int peePodcast::GetNbrTracks()
{
	return _traks->size();
}

peePodcastTrack*  peePodcast::GetTracksAt(unsigned int index)
{

	if(index<_traks->size())
		return _traks->at(index);
	else
		return NULL;
}

void peePodcast::setTitle(const char* title)
{
	_titleUTF8=(char*)malloc(strlen(title)+1);
	strcpy(_titleUTF8,title);

	_directory=(char*)malloc(strlen(title)+1);
	strcpy(_directory,title);

	toolsCleanUTF8(_directory);
}

void peePodcast::setImage(const char* img)
{
	_coverHtmplPath=(char*)malloc(strlen(img)+1);
	strcpy(_coverHtmplPath,img);
}

void peePodcast::print() {

	cout << _titleUTF8 <<  endl;

}

peePodcastTrack* peePodcast::GetTrackByTitle(const char* title)
{
	for(unsigned int ii=0;ii<_traks->size();ii++)
	{
		if(strcmp(_traks->at(ii)->_title,title)==0)
		{
			return _traks->at(ii);
		}
	}
	return NULL;
}
