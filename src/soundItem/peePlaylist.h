/*
 * peePlaylist.h
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#ifndef SOUNDITEM_PEEPLAYLIST_H_
#define SOUNDITEM_PEEPLAYLIST_H_

#include "peeBase.h"
#include "peeTrack.h"

class peeTrack;

class peePlaylist: public peeBase {
public:
	peePlaylist();
	virtual ~peePlaylist();
	peePlaylist(const char*  id,const char* name,const char* coverHtmplPath);
	void fetchTracks();

	char* _name;
	char* _coverHtmplPath;
	std::vector<peeTrack*>* _tracks;
	int	  _currentTrack;

	void print();
};

#endif /* SOUNDITEM_PEEPLAYLIST_H_ */
