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
#include <memory>

#include "peePodcast.h"
#include "peeBase.h"
#include "tools.h"

using namespace std;

peePodcast::peePodcast() {
	// TODO Auto-generated constructor stub
	_id=NULL;
	_titleUTF8=NULL;
	_directory=NULL;
	_coverHtmplPath=NULL;
}

peePodcast::peePodcast(char* htmlSource){
	// TODO Auto-generated constructor stub
	_titleUTF8="";
	_directory="";
	_coverHtmplPath="";
	_htmlSource=(char*)malloc(strlen(htmlSource)+1);
	strcpy(_htmlSource,htmlSource);


	_traks=toolsGetUserPodcastTracks(this,_htmlSource);
	printf("Podcast created\n");
}

peePodcast::~peePodcast() {
	// TODO Auto-generated destructor stub
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
