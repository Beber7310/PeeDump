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

#include <peeTrack.h>

using namespace std;

peeTrack::peeTrack(const char* id,const char* title,int length,peeAlbum* pAlbum) {
	// TODO Auto-generated constructor stub
	_id=(char*)malloc(strlen(id)+1);;
	_title=(char*)malloc(strlen(title)+1);

	strcpy(_id,id);
	strcpy(_title,title);
	_length=length;
	_pAlbum=pAlbum;
}

peeTrack::~peeTrack() {
	// TODO Auto-generated destructor stub
}

void peeTrack::print() {
	cout << "   " << _title << " " << _pAlbum->_albumName << "(" << _id << ")" <<  endl;
}
