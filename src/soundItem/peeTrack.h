/*
 * peeTrack.h
 *
 *  Created on: 12 avr. 2017
 *      Author: Bertrand
 */

#ifndef SOUNDITEM_PEETRACK_H_
#define SOUNDITEM_PEETRACK_H_

#include "peeBase.h"
#include "peeAlbum.h"

class peeAlbum;

class peeTrack : public peeBase {
public:
	peeTrack(const char* id,const char* title,int length, peeAlbum* pAlbum);
	virtual ~peeTrack();

	int _length;
	peeAlbum* _pAlbum;

	void print();
};

#endif /* SOUNDITEM_PEETRACK_H_ */
