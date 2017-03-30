/*
 * peePodcast.h
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#ifndef SOUNDITEM_peePodcast_H_
#define SOUNDITEM_peePodcast_H_

#include <vector>

class peePodcast;

#include "peeBase.h"
#include "peePodcastTrack.h"

class peePodcast: public peeBase {
public:
	peePodcast();
	virtual ~peePodcast();
	peePodcast( char* htmlSource,int age,int time);
	void updatePodcast();

	void setTitle(const char*);
	void setImage(const char*);

	peePodcastTrack* GetTracksAt(unsigned int index);
	peePodcastTrack* GetTrackByTitle(const char* title);
	unsigned int GetNbrTracks();

	char* _titleUTF8;
	char* _directory;
	char* _coverHtmplPath;
	char* _htmlSource; // point on the xml file


	int _maxAge;
	int _minLength;
	void print();
private:
	std::vector<peePodcastTrack*>* _traks;
};

#endif /* SOUNDITEM_peePodcast_H_ */
