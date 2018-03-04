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

	int GetNbrTracksDownloaded();
	void ComputeNbrTracksDownloaded();
	int GetNbrTracks();

	int GetFirstTrack();
	int GetNextTrack();
	void dumpPlaylist();
	int GetFirstMissingTrack();
	int GetNextMissingTrack();

	char* _artisteName;
	char* _albumName;
	int   _nbrTracksDownloaded;
	char* _coverHtmplPath;
	int	  _currentTrack;
	char* _localDir;
	char* _localPathCover;
	char* _genre;

	std::vector<peeTrack*>* _tracks;

	void fetchTracks();
	void print();
	void downloadPicture();
	void dumpInfo();
	void setGenre(const char* szGenre);
};

#endif /* SOUNDITEM_PEEALBUM_H_ */
