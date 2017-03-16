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

using namespace std;

peePodcast::peePodcast() {
	// TODO Auto-generated constructor stub
	_id=NULL;
	_title=NULL;
	_coverHtmplPath=NULL;
}

peePodcast::~peePodcast() {
	// TODO Auto-generated destructor stub
}

peePodcast::peePodcast(const char* htmlSource){
	// TODO Auto-generated constructor stub
	_title="";
	_coverHtmplPath="";
	_htmlSource=(char*)malloc(strlen(htmlSource)+1);
	_traks=toolsGetUserPodcastTracks(this,_htmlSource);

}

void peePodcast::print() {

	cout << _title <<  endl;

}
