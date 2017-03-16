/*
 * peePo	dcatTrack.cpp
 *
 *  Created on: Mar 16, 2017
 *      Author: dosdab
 */

#include <peePodcatTrack.h>


peePodcatTrack::peePodcatTrack() {
	// TODO Auto-generated constructor stub

}
peePodcatTrack::peePodcatTrack(peePodcast *pParent,time_t	date, char*	title, char* htmlPath) {
	// TODO Auto-generated constructor stub
	_date=date;
	_title=title;
	_htmlPath=htmlPath;

	_localPath=_date + _title;
	_downloaded =false;
}


void peePodcatTrack::checkDownload(void)
{
	if(exist (_localPath))
	{
		_downloaded=true;
	}
	else
	{
		Download();
	}
}

void peePodcatTrack::Download(void)
{
	char* mp3;
	new tread()
	{
		mutex_lock();
		//mp3=toolsGetHtml(_htmlPath);
		// we must use wget as it is not html but mp3 (not a string)
		system(wget);
		mutex_release();
	}
}

peePodcatTrack::~peePodcatTrack() {
	// TODO Auto-generated destructor stub
}

