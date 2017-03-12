/*
 * peeAlbum.h
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#ifndef SOUNDITEM_PEEALBUM_H_
#define SOUNDITEM_PEEALBUM_H_

#include "peeBase.h"

class peeAlbum: public peeBase {
public:
	peeAlbum();
	virtual ~peeAlbum();
	peeAlbum(uint32_t id,const char* artisteName,const char* albumName,const char* coverHtmplPath);


	char* _artisteName;
	char* _albumName;
	char* _coverHtmplPath;


	void print();
};

#endif /* SOUNDITEM_PEEALBUM_H_ */
