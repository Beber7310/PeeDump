/*
 * configuration.h
 *
 *  Created on: 7 avr. 2017
 *      Author: Bertrand
 */

#ifndef INC_CONFIGURATION_H_
#define INC_CONFIGURATION_H_

#define BOOMBOOM_SALON
//#define BOOMBOOM_MAMIE
//#define BOOMBOOM_DAPHNEE


#ifdef BOOMBOOM_SALON
	#define SCREEN_7P
	#define HOMECONTROL
	//#define DOWNLOAD_ROOT_DIR "/mnt/hdd/"
	//#define DOWNLOAD_ROOT_DIR "/home/pi/projects/audio/"
	#define DEEZER
#elif defined(BOOMBOOM_DAPHNEE)
	#define SCREEN_5P
	//#define DOWNLOAD_ROOT_DIR "/home/pi/projects/audio/"
#elif defined(BOOMBOOM_MAMIE)
	#define SCREEN_7P
	//#define DOWNLOAD_ROOT_DIR "/home/pi/projects/audio/"
	#define DEEZER
#endif

#define DOWNLOAD_ROOT_DIR "/home/pi/projects/audio/"

//#define LOG_DEEZER_ENABLE


#endif /* INC_CONFIGURATION_H_ */
