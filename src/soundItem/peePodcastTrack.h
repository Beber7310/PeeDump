/*
 * peePodcatTrack.h
 *
 *  Created on: Mar 16, 2017
 *      Author: dosdab
 */

#ifndef SOUNDITEM_PEEPODCASTTRACK_H_
#define SOUNDITEM_PEEPODCASTTRACK_H_
#include <time.h>
#include <semaphore.h>
class peePodcastTrack;

#include "peeBase.h"
#include "peePodcastTrack.h"
#include "peePodcast.h"

class peePodcastTrack : public peeBase{
public:
	peePodcastTrack();
	peePodcastTrack(peePodcast* pParent,tm* date,const  char* title, const char* htmlPath,int size);
	virtual ~peePodcastTrack();

	void checkDownload(void);

	tm		_date;
	char*	_title;
	char*	_htmlPath;
	char* 	_localPath;
	bool 	_downloaded;
	int 	_size;
};

#endif /* SOUNDITEM_PEEPODCASTTRACK_H_ */
