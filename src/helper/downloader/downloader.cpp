/*
 * downloader.c
 *
 *  Created on: 17 mars 2017
 *      Author: Bertrand
 */
#include "configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <semaphore.h>
#include <pthread.h>
#include <list>
#include <string.h>
#include <time.h>
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
	pthread_setname_np(my_podCastDownload,"Podcast Downloader");

	if(ret != 0) {
		printf("Error: pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}

	return ;
}

bool compare_podcast (const peePodcastTrack* first, const peePodcastTrack* second)
{
	return ( mktime((tm*)(&first->_date)) > mktime((tm*)&second->_date) );
}

int toolsDownloadPodcast(peePodcastTrack* pPodcast)
{
	pthread_mutex_lock(&downloadMutex);
	podcastDownloadList->push_back(pPodcast);
	podcastDownloadList->sort(compare_podcast);
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

		pPodcast->checkDownload();
		if(pPodcast->_downloaded==false)
		{
			strcpy(szPath,DOWNLOAD_ROOT_DIR);
			strcat(szPath,pPodcast->_localPath);

			sprintf(szCmd,"wget \"%s\" -O \"%s\" -q --limit-rate=100k",pPodcast->_htmlPath,szPath);
			system(szCmd);
			printf("%s\n",szCmd);
			system("mpc update");
			pPodcast->checkDownload();
		}
	}
	return NULL;
}

bool  toolsDownloadExist(char * localPath)
{
	if(access( localPath, F_OK ) != -1 )
		return true;
	return false;
}

off_t toolsDownloadFileSize(const char *localPath) {
	struct stat st;

	char szPath[1024];


	if (stat(localPath, &st) == 0)
		return st.st_size;

	return 0;
}
