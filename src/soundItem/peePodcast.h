/*
 * peePodcast.h
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#ifndef SOUNDITEM_peePodcast_H_
#define SOUNDITEM_peePodcast_H_

#include "peeBase.h"
#include "peePodcastTrack.h"
class peePodcast: public peeBase {
public:
	peePodcast();
	virtual ~peePodcast();
	peePodcast(const char* coverHtmplPath);


	char* _title;
	char* _coverHtmplPath;
	char* _htmlSource; // point on the xml file
	std::vector<peePodcastTrack*>* _traks;

	void print();
};

#endif /* SOUNDITEM_peePodcast_H_ */
