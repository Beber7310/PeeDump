/*
 * deezer.h
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#ifndef INC_DEEZER_H_
#define INC_DEEZER_H_

#ifdef __cplusplus
extern "C" {
#endif


#define DEEZER_CMD_START			10
#define DEEZER_CMD_STOP				11
#define DEEZER_CMD_PAUSE			12
#define DEEZER_CMD_RESUME			13

#define DEEZER_CMD_NEXT				20
#define DEEZER_CMD_PREV				21

#define DEEZER_CMD_LOAD_ALBUM		30
#define DEEZER_CMD_LOAD_PLAYLIST	31
#define DEEZER_CMD_LOAD_PODCAST		32
#define DEEZER_CMD_LOAD_PODCAST_MP3	33
#define DEEZER_CMD_LOAD_DIR_MP3		34

#define DEEZER_CMD_REPEAT_MODE_OFF	40
#define DEEZER_CMD_REPEAT_MODE_ONE	41
#define DEEZER_CMD_REPEAT_MODE_ALL	42

#define DEEZER_CMD_RANDOM_OFF		50
#define DEEZER_CMD_RANDOM_ON		51

#define DEEZER_CMD_EXIT				60

int deezerLaunch(char* token);
int deezerPostCommand(uint32_t cmd,const char* arg,const char* name );
char* deezerGetSongName();
#ifdef __cplusplus
	}
#endif

#endif /* INC_DEEZER_H_ */
