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

peePodcast::peePodcast(const char* id,const char* title,const char* coverHtmplPath){
	// TODO Auto-generated constructor stub
	_id=(char*)malloc(strlen(id)+1);;
	_title=(char*)malloc(strlen(title)+1);
	_coverHtmplPath=(char*)malloc(strlen(coverHtmplPath)+1);

	strcpy(_id,id);
	strcpy(_title,title);
	strcpy(_coverHtmplPath,coverHtmplPath);
}

void peePodcast::print() {

	cout << _title << "(" << _id << ")" <<  endl;

}
