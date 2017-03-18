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

	peePodcastTrack* pPodcast;

	while(1)
	{
		sem_wait(&semaphorPodcastDownload);
		pthread_mutex_lock(&downloadMutex);
		pPodcast=podcastDownloadList->front();
		podcastDownloadList->pop_front();
		pthread_mutex_unlock(&downloadMutex);


		sprintf(szCmd,"wget %s -O \"%s\" -q",pPodcast->_htmlPath,pPodcast->_localPath);
		system(szCmd);
	}
	return NULL;
}
