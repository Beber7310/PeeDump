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

peeTrack::peeTrack(const char* id,const char* title,int length,peeAlbum* pAlbum,const char* szAlbum, const char* szArtist,int pos) {
	// TODO Auto-generated constructor stub
	_id=(char*)malloc(strlen(id)+1);;
	_title=(char*)malloc(strlen(title)+1);
	_szAlbum=_pAlbum->_albumName;
	_szArtist=_pAlbum->_artisteName;

	strcpy(_id,id);
	strcpy(_title,title);
	_length=length;
	_pAlbum=pAlbum;
	_pPlaylist=NULL;
	_Position=pos;
}

peeTrack::peeTrack(const char* id, const char* title, int length, peePlaylist* pPlaylist, const char* szAlbum, const char* szArtist,int pos) {
	// TODO Auto-generated constructor stub
	_id=(char*)malloc(strlen(id)+1);
	_title=(char*)malloc(strlen(title)+1);
	_szAlbum=(char*)malloc(strlen(szAlbum)+1);
	_szArtist=(char*)malloc(strlen(szArtist)+1);

	strcpy(_id,id);
	strcpy(_title,title);
	strcpy(_szAlbum,szAlbum);
	strcpy(_szArtist,szArtist);

	_length=length;
	_pPlaylist=pPlaylist;
	_pAlbum=NULL;

	_Position=pos;

}

peeTrack::~peeTrack() {
	// TODO Auto-generated destructor stub
}

void peeTrack::print() {
	/*if(_pAlbum)
		cout << "   " << _title << " " << _pAlbum->_albumName << "(" << _id << ")" <<  endl;*/

			cout << "   " << _title << " " << _szAlbum << "(" << _id << ")" <<  endl;
}
