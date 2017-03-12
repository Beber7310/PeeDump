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

#include "peeAlbum.h"
#include "peeBase.h"

using namespace std;

peeAlbum::peeAlbum() {
	// TODO Auto-generated constructor stub
	_artisteName=NULL;
	_albumName=NULL;
	_coverHtmplPath=NULL;
}

peeAlbum::~peeAlbum() {
	// TODO Auto-generated destructor stub
}

peeAlbum::peeAlbum(uint32_t id,const char* artisteName,const char* albumName,const char* coverHtmplPath){
	// TODO Auto-generated constructor stub
	_id=id;
	_artisteName=(char*)malloc(strlen(artisteName)+1);
	_albumName=(char*)malloc(strlen(albumName)+1);
	_coverHtmplPath=(char*)malloc(strlen(coverHtmplPath)+1);

	strcpy(_albumName,albumName);
	strcpy(_artisteName,artisteName);
	strcpy(_coverHtmplPath,coverHtmplPath);

}

void peeAlbum::print() {

	cout << _artisteName << " " << _albumName << "(" << _id << ")" <<  endl;

}
