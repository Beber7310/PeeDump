/*
 * peePlaylist.cpp
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "configuration.h"
#include "peePlaylist.h"
#include "tools.h"
#include"downloader.h"

using namespace std;

peePlaylist::peePlaylist() {
	// TODO Auto-generated constructor stub
	_id = NULL;
	_name = NULL;
	_coverHtmplPath = NULL;
	_tracks = NULL;
	_currentTrack = 0;
	_nbrTracksDownloaded = 0;
}

peePlaylist::~peePlaylist() {
	// TODO Auto-generated destructor stub
}

peePlaylist::peePlaylist(const char* id, const char* name, const char* coverHtmplPath) {
	// TODO Auto-generated constructor stub
	_id = (char*) malloc(strlen(id) + 1);
	_name = (char*) malloc(strlen(name) + 1);
	_coverHtmplPath = (char*) malloc(strlen(coverHtmplPath) + 1);

	strcpy(_id, id);
	strcpy(_name, name);

	cleanMP3String(_name);

	strcpy(_coverHtmplPath, coverHtmplPath);
	_tracks = NULL;
	_currentTrack = 0;
	_nbrTracksDownloaded = 0;

	_localDir = (char*) malloc(strlen(DOWNLOAD_ROOT_DIR) + strlen(_id) + 20);
	sprintf(_localDir, "%s/mp3/play_%s", DOWNLOAD_ROOT_DIR, _id);
	//toolsCleanUTF8(_localDir);

	_localPathCover = (char*) malloc(strlen(_localDir) + strlen("cover.jpeg") + 20);
	sprintf(_localPathCover, "%s/cover.jpeg", _localDir);

	printf("<PLaylist> %s\n", _name);
}

void peePlaylist::downloadPicture() {
	char szCmd[1024];

	if (!toolsDownloadExist(_localPathCover)) {
		sprintf(szCmd, "mkdir -p \"%s\"", _localDir);
		system(szCmd);

		sprintf(szCmd, "wget \"%s\" -O \"%s\" -q --limit-rate=100k", _coverHtmplPath, _localPathCover);
		system(szCmd);
		printf("%s\n", szCmd);
	}
}

void peePlaylist::print() {
	cout << _name << "(" << _id << ")" << endl;
	if (_tracks != NULL) {
		for (vector<peeTrack*>::iterator it = _tracks->begin(); it != _tracks->end(); it++) {
			(*it)->print();
		}
	}
}

void peePlaylist::fetchTracks() {
	if (_tracks == NULL) {
		_tracks = toolsGetUserPlaylistTracks(this);
		ComputeNbrTracksDownloaded();
		downloadPicture();
		dumpPlaylist();
	}
}

int peePlaylist::GetNbrTracksDownloaded() {
	return _nbrTracksDownloaded;
}

void peePlaylist::ComputeNbrTracksDownloaded() {
	_nbrTracksDownloaded = 0;

	if (_tracks != NULL) {
		for (vector<peeTrack*>::iterator it = _tracks->begin(); it != _tracks->end(); it++) {
			if ((*it)->checkDownload()) {
				_nbrTracksDownloaded++;
			} else {
				printf("Not downloaded: %s %s\n", (*it)->_szAlbum, (*it)->_szArtist);
			}
		}
	}

}

int peePlaylist::GetNbrTracks() {
	int nbr = 0;

	if (_tracks != NULL) {
		nbr = _tracks->size();
	}
	return nbr;
}

int peePlaylist::GetFirstTrack() {
	for (_currentTrack = 0; _currentTrack < _tracks->size(); _currentTrack++) {

		if (!_tracks->at(_currentTrack)->checkDownload()) {
			return _currentTrack;
		}
	}
	_currentTrack = 0;
	return _currentTrack;

}

int peePlaylist::GetNextTrack() {
	for (_currentTrack++; _currentTrack < _tracks->size(); _currentTrack++) {
		if (!_tracks->at(_currentTrack)->checkDownload()) {
			return _currentTrack;
		}
	}

	return _currentTrack;
}

/*
 void peePlaylist::dumpPlaylist2() {
 FILE *stream;
 char szFineName[256];

 sprintf(szFineName, "%s/playlist/%s.m3u", DOWNLOAD_ROOT_DIR, _name);
 toolsCleanUTF8(szFineName);
 stream = fopen(szFineName, "w");
 if (stream == NULL) {
 printf("Unable to open %s %i\n", szFineName, errno);
 exit(EXIT_FAILURE);
 }

 for (vector<peeTrack*>::iterator it = _tracks->begin();
 it != _tracks->end(); it++) {
 fprintf(stream, "../%s\n",
 &(*it)->_localPath[strlen(DOWNLOAD_ROOT_DIR)]);
 }

 fclose(stream);
 }
 */

void peePlaylist::dumpPlaylist() {
	FILE *stream;
	char szFineName[256];

	/*
	 * M3U playlist
	 */
	sprintf(szFineName, "%s/playlists/%s.m3u", DOWNLOAD_ROOT_DIR, _name);
	toolsCleanUTF8(szFineName);
	stream = fopen(szFineName, "w");
	if (stream == NULL) {
		printf("Unable to open %s %i\n", szFineName, errno);
		exit(EXIT_FAILURE);
	}

	for (vector<peeTrack*>::iterator it = _tracks->begin(); it != _tracks->end(); it++) {
		fprintf(stream, "../%s\n", &(*it)->_localPath[strlen(DOWNLOAD_ROOT_DIR) + 1]);
	}

	fclose(stream);

	/*
	 *  Piz playlist
	 */

	sprintf(szFineName, "%s/playlists/%s.piz", DOWNLOAD_ROOT_DIR, _name);
	toolsCleanUTF8(szFineName);
	stream = fopen(szFineName, "w");
	if (stream == NULL) {
		printf("Unable to open %s %i\n", szFineName, errno);
		exit(EXIT_FAILURE);
	}

	fprintf(stream, "<PLAYLIST>%s\n", _name);
	//fprintf(stream, "<COVER>%s\n", _localPathCover);
	fprintf(stream, "<COVER>mp3/play_%s/cover.jpeg\n", _id);

	for (vector<peeTrack*>::iterator it = _tracks->begin(); it != _tracks->end(); it++) {
		fprintf(stream, "%s %s\n", &(*it)->_localPath[strlen(DOWNLOAD_ROOT_DIR) + 1], (*it)->_title);
	}

	fclose(stream);

}

