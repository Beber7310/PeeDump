/*
 * peePodcatTrack.h
 *
 *  Created on: Mar 16, 2017
 *      Author: dosdab
 */

#ifndef SOUNDITEM_PEEPODCATTRACK_H_
#define SOUNDITEM_PEEPODCATTRACK_H_

class peePodcatTrack {
public:
	peePodcatTrack();
	peePodcatTrack(peePodcast *pParent,time_t	date, char*	title, char* htmlPath);

	virtual ~peePodcatTrack();

	static mutex;

	time_t	_date;
	char*	_title;
	char*	_htmlPath;
	char* 	_localPath
	bool 	_downloaded;
};

#endif /* SOUNDITEM_PEEPODCATTRACK_H_ */
