/*
 * downloader.h
 *
 *  Created on: 17 mars 2017
 *      Author: Bertrand
 */

#ifndef HELPER_DOWNLOADER_DOWNLOADER_H_
#define HELPER_DOWNLOADER_DOWNLOADER_H_

#define DOWNLOAD_ROOT_DIR "/mnt/usbdrive/"

void toolsDownloadInit(void);
int toolsDownloadPodcast(peePodcastTrack* peePodcast);
bool  toolsDownloadExist(char * localPath);
off_t toolsDownloadFileSize(const char *filename);
#endif /* HELPER_DOWNLOADER_DOWNLOADER_H_ */
