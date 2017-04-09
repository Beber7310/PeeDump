/*
 * main.h
 *
 *  Created on: 12 mars 2017
 *      Author: Bertrand
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include <vector>

#include "http_fetcher.h"		/* Must include this to use HTTP Fetcher */
#include "deezer.h"
#include "peeAlbum.h"
#include "peePlaylist.h"
#include "peePodcast.h"

struct stAppContext {
	char 		gToken[128];
	uint32_t	gUser;
	std::vector<peeAlbum*>*    Albums;
	std::vector<peePlaylist*>* Playlist;
	std::vector<peePodcast*>* Podcasts;
};

struct stMouse {
	int32_t	x,y;
	int  update;
	bool t;
	bool firstTouch;
	bool LastTouch;
	bool Click;
};


typedef struct stAppContext stAppContext;

extern stAppContext appContext;

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
	}
#endif

#endif /* INC_MAIN_H_ */
