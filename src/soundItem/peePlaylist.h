/*
 * peePlaylist.h
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#ifndef SOUNDITEM_PEEPLAYLIST_H_
#define SOUNDITEM_PEEPLAYLIST_H_

#include "peeBase.h"

class peePlaylist: public peeBase {
public:
	peePlaylist();
	virtual ~peePlaylist();
	peePlaylist(uint32_t id,const char* name);

	char* _name;

	void print();
};

#endif /* SOUNDITEM_PEEPLAYLIST_H_ */
