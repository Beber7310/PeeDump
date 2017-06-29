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
	int _nbrTracksDownloaded;
	void print();
	int GetNbrTracksDownloaded();

	void ComputeNbrTracksDownloaded();
	void dumpPlaylist();
	int GetNbrTracks();

	int GetFirstTrack();
	int GetNextTrack();
};

#endif /* SOUNDITEM_PEEPLAYLIST_H_ */
