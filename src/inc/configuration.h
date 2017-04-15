/*
 * configuration.h
 *
 *  Created on: 7 avr. 2017
 *      Author: Bertrand
 */

#ifndef INC_CONFIGURATION_H_
#define INC_CONFIGURATION_H_

//#define BOOMBOOM_SALON
#define BOOMBOOM_DAPHNEE


#ifdef BOOMBOOM_SALON
	#define SCREEN_7P
	#define DOWNLOAD_ROOT_DIR "/mnt/usbdrive/"
#elif defined(BOOMBOOM_DAPHNEE)
	#define SCREEN_5P
	#define DOWNLOAD_ROOT_DIR "/home/pi/projects/audio/"
#endif



#endif /* INC_CONFIGURATION_H_ */
