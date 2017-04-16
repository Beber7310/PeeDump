/*
 * peeAlbum.h
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */
#include <vector>

#ifndef SOUNDITEM_PEEALBUM_H_
#define SOUNDITEM_PEEALBUM_H_

#include "peeBase.h"
#include "peeTrack.h"

class peeTrack;

class peeAlbum: public peeBase {
public:
	peeAlbum();
	virtual ~peeAlbum();
	peeAlbum(const char*  id,const char* artisteName,const char* albumName,const char* coverHtmplPath);


	char* _artisteName;
	char* _albumName;
	char* _coverHtmplPath;
	int	  _currentTrack;
	std::vector<peeTrack*>* _tracks;

	void fetchTracks();
	void print();
};

#endif /* SOUNDITEM_PEEALBUM_H_ */
