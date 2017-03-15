/*
 * peePodcast.h
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#ifndef SOUNDITEM_peePodcast_H_
#define SOUNDITEM_peePodcast_H_

#include "peeBase.h"

class peePodcast: public peeBase {
public:
	peePodcast();
	virtual ~peePodcast();
	peePodcast(const char* id,const char* title,const char* coverHtmplPath);


	char* _title;
	char* _coverHtmplPath;


	void print();
};

#endif /* SOUNDITEM_peePodcast_H_ */
