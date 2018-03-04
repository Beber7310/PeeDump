/*
 * peeAlbum.cpp
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#include "configuration.h"

#include "peeAlbum.h"
#include "peeBase.h"
#include "tools.h"
#include"downloader.h"

using namespace std;

peeAlbum::peeAlbum() {
	// TODO Auto-generated constructor stub
	_id = NULL;
	_artisteName = NULL;
	_albumName = NULL;
	_coverHtmplPath = NULL;
	_tracks = NULL;
	_currentTrack = 0;
	_nbrTracksDownloaded = 0;
	_localPathCover = NULL;
	_localDir = NULL;
}

peeAlbum::~peeAlbum() {
	// TODO Auto-generated destructor stub
}
void cleanAlbumString(char* str) {
	for (unsigned int ii = 0; ii < strlen(str); ii++) {
		if (str[ii] == '\"')
			str[ii] = '\'';

		if (str[ii] == '`')
			str[ii] = '\'';

		if (str[ii] == '\\')
			str[ii] = ' ';

		if (str[ii] == '/')
			str[ii] = ' ';
	}

}

peeAlbum::peeAlbum(const char* id, const char* artisteName, const char* albumName, const char* coverHtmplPath) {
	// TODO Auto-generated constructor stub
	_id = (char*) malloc(strlen(id) + 1);
	;
	_artisteName = (char*) malloc(strlen(artisteName) + 1);
	_albumName = (char*) malloc(strlen(albumName) + 1);
	_coverHtmplPath = (char*) malloc(strlen(coverHtmplPath) + 1);
	_genre = NULL;

	strcpy(_id, id);
	strcpy(_albumName, albumName);
	strcpy(_artisteName, artisteName);
	strcpy(_coverHtmplPath, coverHtmplPath);

	cleanAlbumString(_albumName);
	cleanAlbumString(_artisteName);

	_localDir = (char*) malloc(strlen(DOWNLOAD_ROOT_DIR) + strlen(_id) + 20);
	sprintf(_localDir, "%s/mp3/alb_%s", DOWNLOAD_ROOT_DIR, _id);
	//toolsCleanUTF8(_localDir);

	_localPathCover = (char*) malloc(strlen(_localDir) + strlen("cover.jpeg") + 20);
	sprintf(_localPathCover, "%s/cover.jpeg", _localDir);

	_currentTrack = 0;
	_tracks = NULL;
	_nbrTracksDownloaded = 0;

	printf("<Album> %s\n", _albumName);

}

void peeAlbum::setGenre(const char* szGenre) {
	_genre = (char*) malloc(strlen(szGenre) + 1);
	strcpy(_genre, szGenre);
}

void peeAlbum::fetchTracks() {
	if (_tracks == NULL) {
		_tracks = toolsGetUserAlbumTracks(this);
		ComputeNbrTracksDownloaded();
		downloadPicture();
		dumpPlaylist();
		dumpInfo();
	}
}

void peeAlbum::print() {

	cout << _artisteName << " " << _albumName << "(" << _id << ")" << endl;
	if (_tracks != NULL) {
		for (vector<peeTrack*>::iterator it = _tracks->begin(); it != _tracks->end(); it++) {
			(*it)->print();
		}
	}
}

void peeAlbum::downloadPicture() {
	char szCmd[1024];

	if (!toolsDownloadExist(_localPathCover)) {
		sprintf(szCmd, "mkdir -p \"%s\"", _localDir);
		system(szCmd);

		sprintf(szCmd, "wget \"%s\" -O \"%s\" -q --limit-rate=100k", _coverHtmplPath, _localPathCover);
		system(szCmd);
		printf("%s\n", szCmd);
	}
}

void peeAlbum::dumpInfo() {
	FILE *stream;
	char szFineName[256];

	sprintf(szFineName, "%s/info.dat", _localDir);
	stream = fopen(szFineName, "w");
	if (stream == NULL) {
		printf("Unable to open %s %i\n", szFineName, errno);
		exit(EXIT_FAILURE);
	}

	fprintf(stream, "<ARTISTE>%s\n", _artisteName);
	fprintf(stream, "<ALBUM>%s\n", _albumName);

	for (vector<peeTrack*>::iterator it = _tracks->begin(); it != _tracks->end(); it++) {
		fprintf(stream, "<TRACK>%s %s\n", (*it)->_id, (*it)->_title);
		printf("%s %s\n", (*it)->_id, (*it)->_title);
	}

	fclose(stream);

}

int peeAlbum::GetNbrTracksDownloaded() {
	return _nbrTracksDownloaded;
}

void peeAlbum::ComputeNbrTracksDownloaded() {
	_nbrTracksDownloaded = 0;

	if (_tracks != NULL) {
		for (vector<peeTrack*>::iterator it = _tracks->begin(); it != _tracks->end(); it++) {
			if ((*it)->checkDownload()) {
				_nbrTracksDownloaded++;
			} else {
				printf("Not downloaded: %s %s %s\n", (*it)->_szArtist, (*it)->_szAlbum, (*it)->_title);
			}
		}
	}

}

int peeAlbum::GetNbrTracks() {
	int nbr = 0;

	if (_tracks != NULL) {
		nbr = _tracks->size();
	}
	return nbr;
}

int peeAlbum::GetFirstTrack() {
	_currentTrack = 0;
	for (unsigned int ii = _currentTrack; ii < _tracks->size(); ii++) {

		if (!_tracks->at(ii)->checkDownload()) {
			return _currentTrack;
		}
		_currentTrack++;
	}
	return _currentTrack;

}

int peeAlbum::GetFirstMissingTrack() {
	_currentTrack = 0;
	for (unsigned int ii = _currentTrack; ii < _tracks->size(); ii++) {

		if (!_tracks->at(ii)->checkDownload()) {
			return _currentTrack;
		}
		_currentTrack++;
	}
	return _currentTrack;

}

int peeAlbum::GetNextMissingTrack() {
	_currentTrack++;
	for (unsigned int ii = _currentTrack; ii < _tracks->size(); ii++) {

		if (!_tracks->at(_currentTrack)->checkDownload()) {
			return _currentTrack;
		}
		_currentTrack++;
	}
	return _currentTrack;
}

int peeAlbum::GetNextTrack() {

	for (unsigned int ii = _currentTrack; ii < _tracks->size(); ii++) {

		if (!_tracks->at(ii)->checkDownload()) {
			return _currentTrack;
		}
		_currentTrack++;
	}
	return _currentTrack;
}

void peeAlbum::dumpPlaylist() {
	FILE *stream;
	char szFineName[256];

	/*
	 * M3U playlist
	 */
	sprintf(szFineName, "%s/album/%s_%s.m3u", DOWNLOAD_ROOT_DIR, _artisteName, _albumName);
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
	sprintf(szFineName, "%s/album/%s_%s.piz", DOWNLOAD_ROOT_DIR, _artisteName, _albumName);
	toolsCleanUTF8(szFineName);
	stream = fopen(szFineName, "w");
	if (stream == NULL) {
		printf("Unable to open %s %i\n", szFineName, errno);
		exit(EXIT_FAILURE);
	}

	fprintf(stream, "<ARTISTE>%s\n", _artisteName);
	fprintf(stream, "<ALBUM>%s\n", _albumName);

	//fprintf(stream, "<COVER>%s\n", _localPathCover);
	fprintf(stream, "<COVER>mp3/alb_%s/cover.jpeg\n", _id);
	fprintf(stream, "<GENRE>%s\n", _genre);

	for (vector<peeTrack*>::iterator it = _tracks->begin(); it != _tracks->end(); it++) {
		fprintf(stream, "%s %s\n", &(*it)->_localPath[strlen(DOWNLOAD_ROOT_DIR) + 1], (*it)->_title);
	}

	fclose(stream);

}
