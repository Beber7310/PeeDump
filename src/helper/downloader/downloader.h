/*
 * downloader.h
 *
 *  Created on: 17 mars 2017
 *      Author: Bertrand
 */

#ifndef HELPER_DOWNLOADER_DOWNLOADER_H_
#define HELPER_DOWNLOADER_DOWNLOADER_H_


void toolsDownloadInit(void);
int toolsDownloadPodcast(peePodcastTrack* peePodcast);
bool  toolsDownloadExist(char * localPath);
off_t toolsDownloadFileSize(const char *filename);
bool compare_podcast (const peePodcastTrack* first, const peePodcastTrack* second);
#endif /* HELPER_DOWNLOADER_DOWNLOADER_H_ */
