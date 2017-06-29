/*
 * peeTrack.h
 *
 *  Created on: 12 avr. 2017
 *      Author: Bertrand
 */

#ifndef SOUNDITEM_PEETRACK_H_
#define SOUNDITEM_PEETRACK_H_
#include <vector>

#include "peeBase.h"
#include "peeAlbum.h"
#include "peePlaylist.h"

class peeAlbum;
class peePlaylist;

class peeTrack : public peeBase {
public:
	peeTrack(const char* id,const char* title,int length, peeAlbum*    pAlbum,   const char* szAlbum, const char* szArtist, int pos);
	peeTrack(const char* id,const char* title,int length, peePlaylist* pPlaylist,const char* szAlbum, const char* szArtist,int pos);

	virtual ~peeTrack();
	bool checkDownload(void);

	int _length;
	peeAlbum* _pAlbum;
	peePlaylist* _pPlaylist;
	int _Position;

	char* _szAlbum;
	char* _szArtist;
	char* _localPath;
	char* _localDir;
	void print();

};

#endif /* SOUNDITEM_PEETRACK_H_ */
