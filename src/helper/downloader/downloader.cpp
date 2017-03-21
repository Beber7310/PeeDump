/*
 * downloader.c
 *
 *  Created on: 17 mars 2017
 *      Author: Bertrand
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#include <list>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include"peePodcast.h"
#include"downloader.h"

using namespace std;

static sem_t 			semaphorPodcastDownload;
static pthread_mutex_t 	downloadMutex;
list<peePodcastTrack*>* podcastDownloadList;




void* toolsDownloadPodcastThread(void * p);

void toolsDownloadInit(void)
{
	pthread_t my_podCastDownload;

	sem_init(&semaphorPodcastDownload,0,0);
	pthread_mutex_init(&downloadMutex, NULL);
	podcastDownloadList=new list<peePodcastTrack*>();

	int ret;

	ret =  pthread_create(&my_podCastDownload, NULL, &toolsDownloadPodcastThread, NULL);
	if(ret != 0) {
		printf("Error: pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}

	return ;
}

int toolsDownloadPodcast(peePodcastTrack* pPodcast)
{
	pthread_mutex_lock(&downloadMutex);
	podcastDownloadList->push_back(pPodcast);
	pthread_mutex_unlock(&downloadMutex);
	sem_post(&semaphorPodcastDownload);
}

void* toolsDownloadPodcastThread(void * p)
{
	char szCmd[1024];
	char szPath[1024];

	peePodcastTrack* pPodcast;

	while(1)
	{
	
		sem_wait(&semaphorPodcastDownload);
		pthread_mutex_lock(&downloadMutex);
		pPodcast=podcastDownloadList->front();
		podcastDownloadList->pop_front();
		pthread_mutex_unlock(&downloadMutex);

		strcpy(szPath,DOWNLOAD_ROOT_DIR);
		strcat(szPath,pPodcast->_localPath);
		
		sprintf(szCmd,"wget %s -O \"%s\" -q",pPodcast->_htmlPath,szPath);
		//printf("%s\n",szCmd);
		system(szCmd);
		system("mpc update");
		pPodcast->_downloaded=true;

	}
	return NULL;
}

bool  toolsDownloadExist(char * localPath)
{
	char szPath[1024];

	strcpy(szPath,DOWNLOAD_ROOT_DIR);
	strcat(szPath,localPath);

	if(access( szPath, F_OK ) != -1 )
		return true;
	return false;
}

off_t toolsDownloadFileSize(const char *localPath) {
	struct stat st;

	char szPath[1024];

	strcpy(szPath,DOWNLOAD_ROOT_DIR);
	strcat(szPath,localPath);

	if (stat(szPath, &st) == 0)
		return st.st_size;

	return 0;
}
