/*
 * deezer.c
 *
 *  Created on: 10 mars 2017
 *      Author: Bertrand
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include "deezer.h"
#include "configuration.h"
#include "peeAlbum.h"
#include "tools.h"

#define LOG_DEEZER_ENABLE

#ifdef _WIN32
#include <Windows.h>
static __inline int sleep(int s) {
	Sleep(1000*s);
	return 0;
}
#else
#include <unistd.h>
#endif

#include <deezer-connect.h>
#include <deezer-player.h>

static sem_t semaphorDeezer;
static sem_t semaphorRecord;

static bool mpcPause=false;

#define log(fmt, ...) printf("[%s:%d]" fmt, __FILE__, __LINE__, ##__VA_ARGS__);


struct deezerCmd_st
{
	uint32_t Cmd;
	const void* Arg;
	const char* NameToDisplay;
}deezerCmd[16];

static int 		deezerCmdIndexWr=0;
static int 		deezerCmdIndexRd=0;
static const char*	szCurrentSong;

static char	szNone[]="";
static char	szLoading[]="Loading...";

typedef struct {
	int                   nb_track_played;
	bool                  is_playing;
	bool                  is_ready;
	char*                 sz_content_url;
	int                   activation_count;
	dz_connect_handle     dzconnect;
	dz_player_handle      dzplayer;
	dz_queuelist_repeat_mode_t repeat_mode;
	bool                  is_shuffle_mode;
	bool                  is_playing_album;
	bool                  is_playing_playlist;
	peeAlbum*			  pAlbum;
	peePlaylist*		  pPlaylist;
	bool                  is_underflow;
} app_context;


typedef app_context *app_context_handle;
static int print_version    = true;
static int is_shutting_down = false;
static int print_device_id  = true;
static app_context_handle app_ctxt = NULL;


#define YOUR_APPLICATION_ID      "302584"             //"227402"     // SET YOUR APPLICATION ID
#define YOUR_APPLICATION_NAME    "BoomBoomBox" // SET YOUR APPLICATION NAME
#define YOUR_APPLICATION_VERSION "00001"      // SET YOUR APPLICATION VERSION
#define USER_CACHE_PATH          "/var/tmp/dzrcache_NDK_SAMPLE" // SET THE USER CACHE PATH, This pasth must already exist



static void app_commands_get_next();
static void app_change_content(char * content);
static void app_load_content();
static void app_load_album(peeAlbum* pAlbum);
static void app_playback_start_or_stop();
static void app_playback_pause_or_resume();
static void app_playback_next();
static void app_playback_previous();
static void app_playback_toogle_repeat();
static void app_playback_toogle_random();
static void app_playback_random_on();
static void app_playback_random_off();
static void app_shutdown();

static void dz_connect_on_deactivate(void* delegate,
		void* operation_userdata,
		dz_error_t status,
		dz_object_handle result);
static void dz_player_on_deactivate(void* delegate,
		void* operation_userdata,
		dz_error_t status,
		dz_object_handle result);

// callback for dzconnect events
static void app_connect_onevent_cb(dz_connect_handle handle,
		dz_connect_event_handle event,
		void* delegate);
// callback for dzplayer events
static void app_player_onevent_cb(dz_player_handle handle,
		dz_player_event_handle event,
		void* supervisor);

void* mainDeezer(void* voidtoken) {
	char* token=(char*)voidtoken;

	szCurrentSong=szNone;

	struct dz_connect_configuration config;
	dz_error_t dzerr = DZ_ERROR_NO_ERROR;

	if (print_version) {
		log("<-- Deezer native SDK Version : %s\n", dz_connect_get_build_id());
	}

	memset(&config, 0, sizeof(struct dz_connect_configuration));

	config.app_id            = YOUR_APPLICATION_ID;      // SET YOUR APPLICATION ID
	config.product_id        = YOUR_APPLICATION_NAME;    // SET YOUR APPLICATION NAME
	config.product_build_id  = YOUR_APPLICATION_VERSION; // SET YOUR APPLICATION VERSION
	config.user_profile_path = USER_CACHE_PATH;          // SET THE USER CACHE PATH
	config.connect_event_cb  = app_connect_onevent_cb;

	log("--> Application ID:    %s\n", config.app_id);
	log("--> Product ID:        %s\n", config.product_id);
	log("--> Product BUILD ID:  %s\n", config.product_build_id);
	log("--> User Profile Path: %s\n", config.user_profile_path);
	log("--> User token:        <%s>\n", token);


	// Nanoplayer only supports one argument which is the content description
	/* if (argc < 2) {
        log("Please give the content as argument like:\n");
        log("\t \"dzmedia:///track/10287076\"      (Single track example)\n");
        log("\t \"dzmedia:///album/607845\"        (Album example)\n");
        log("\t \"dzmedia:///playlist/1363560485\" (Playlist example)\n");
        log("\t \"dzradio:///radio-223\"           (Radio example)\n");
        log("\t \"dzradio:///user-743548285\"      (User Mix example)\n");
        return -1;
    }*/

	app_ctxt = (app_context_handle)calloc(1,sizeof(app_context));

	//This is hox to set music! --->
#ifdef DEEZER
	app_change_content("dzmedia:///album/607845");

	app_ctxt->dzconnect = dz_connect_new(&config);
	app_ctxt->is_ready=false;

	if (app_ctxt->dzconnect == NULL) {
		log("dzconnect null\n");
		return NULL;
	}

	if (print_device_id) {
		log("Device ID: %s\n", dz_connect_get_device_id(app_ctxt->dzconnect));
	}

	dzerr = dz_connect_debug_log_disable(app_ctxt->dzconnect);
	if (dzerr != DZ_ERROR_NO_ERROR) {
		log("dz_connect_debug_log_disable error\n");
		return NULL;
	}

	dzerr = dz_connect_activate(app_ctxt->dzconnect, app_ctxt);
	if (dzerr != DZ_ERROR_NO_ERROR) {
		log("dz_connect_activate error\n");
		return NULL;
	}
	app_ctxt->activation_count++;

	/* Calling dz_connect_cache_path_set()
	 * is mandatory in order to have the attended behavior */
	dz_connect_cache_path_set(app_ctxt->dzconnect, NULL, NULL, USER_CACHE_PATH);

	app_ctxt->dzplayer = dz_player_new(app_ctxt->dzconnect);
	if (app_ctxt->dzplayer == NULL) {
		log("dzplayer null\n");
		return NULL;
	}

	dzerr = dz_player_activate(app_ctxt->dzplayer, app_ctxt);
	if (dzerr != DZ_ERROR_NO_ERROR) {
		log("dz_player_activate error\n");
		return NULL;
	}
	app_ctxt->activation_count++;

	dzerr = dz_player_set_event_cb(app_ctxt->dzplayer, app_player_onevent_cb);
	if (dzerr != DZ_ERROR_NO_ERROR) {
		log("dz_player_set_event_cb error\n");
		return NULL;
	}

	dzerr = dz_player_set_output_volume(app_ctxt->dzplayer, NULL, NULL, 20);
	if (dzerr != DZ_ERROR_NO_ERROR) {
		log("dz_player_set_output_volume error\n");
		return NULL;
	}

	dzerr = dz_player_set_crossfading_duration(app_ctxt->dzplayer, NULL, NULL, 3000);
	if (dzerr != DZ_ERROR_NO_ERROR) {
		log("dz_player_set_crossfading_duration error\n");
		return NULL;
	}

	app_ctxt->repeat_mode = DZ_QUEUELIST_REPEAT_MODE_OFF;
	app_ctxt->is_shuffle_mode = false;

	dzerr = dz_connect_set_access_token(app_ctxt->dzconnect,NULL, NULL, token);
	if (dzerr != DZ_ERROR_NO_ERROR) {
		log("dz_connect_set_access_token error\n");
		return NULL;
	}

	/* Calling dz_connect_offline_mode(FALSE) is mandatory to force the login */
	dzerr = dz_connect_offline_mode(app_ctxt->dzconnect, NULL, NULL, false);
	if (dzerr != DZ_ERROR_NO_ERROR) {
		log("dz_connect_offline_mode error\n");
		return NULL;
	}
#else
	app_ctxt->activation_count++;
#endif


	while ((app_ctxt->activation_count > 0)) {
		// Get the next user action only if not shutting down.
		if (!is_shutting_down) {
			app_commands_get_next();
		}
	}

	if (app_ctxt->dzplayer) {
		log("-- FREE PLAYER @ %p --\n",app_ctxt->dzplayer);
		dz_object_release((dz_object_handle)app_ctxt->dzplayer);
		app_ctxt->dzplayer = NULL;
	}

	if (app_ctxt->dzconnect) {
		log("-- FREE CONNECT @ %p --\n",app_ctxt->dzconnect);
		dz_object_release((dz_object_handle)app_ctxt->dzconnect);
		app_ctxt->dzconnect = NULL;
	}

	log("-- shutdowned --\n");

	if (app_ctxt) {
		free(app_ctxt);
		app_ctxt = NULL;
	}

	pthread_exit(NULL);
	return 0;
}

static void app_shutdown() {
	log("SHUTDOWN (1/2) - dzplayer = %p\n",app_ctxt->dzplayer);
	if (app_ctxt->dzplayer)
		dz_player_deactivate(app_ctxt->dzplayer, dz_player_on_deactivate, NULL);
	log("SHUTDOWN (2/2) - dzconnect = %p\n",app_ctxt->dzconnect);
	if (app_ctxt->dzconnect)
		dz_connect_deactivate(app_ctxt->dzconnect, dz_connect_on_deactivate, NULL);
}

void app_connect_onevent_cb(dz_connect_handle handle,
		dz_connect_event_handle event,
		void* delegate) {

	dz_connect_event_t type = dz_connect_event_get_type(event);

	app_context_handle context = (app_context_handle)delegate;

	switch (type) {
		case DZ_CONNECT_EVENT_USER_OFFLINE_AVAILABLE:
			log("(App:%p) ++++ CONNECT_EVENT ++++ USER_OFFLINE_AVAILABLE\n",context);
			break;

		case DZ_CONNECT_EVENT_USER_ACCESS_TOKEN_OK:
		{
			const char* szAccessToken;
			szAccessToken = dz_connect_event_get_access_token(event);
			log("(App:%p) ++++ CONNECT_EVENT ++++ USER_ACCESS_TOKEN_OK Access_token : %s\n",context, szAccessToken);
		}
		break;

		case DZ_CONNECT_EVENT_USER_ACCESS_TOKEN_FAILED:
			log("(App:%p) ++++ CONNECT_EVENT ++++ USER_ACCESS_TOKEN_FAILED\n",context);
			break;

		case DZ_CONNECT_EVENT_USER_LOGIN_OK:
			log("(App:%p) ++++ CONNECT_EVENT ++++ USER_LOGIN_OK\n",context);
			app_ctxt->is_ready=true;
			//app_load_content();
			break;

		case DZ_CONNECT_EVENT_USER_NEW_OPTIONS:
			log("(App:%p) ++++ CONNECT_EVENT ++++ USER_NEW_OPTIONS\n",context);
			break;

		case DZ_CONNECT_EVENT_USER_LOGIN_FAIL_NETWORK_ERROR:
			log("(App:%p) ++++ CONNECT_EVENT ++++ USER_LOGIN_FAIL_NETWORK_ERROR\n",context);
			break;

		case DZ_CONNECT_EVENT_USER_LOGIN_FAIL_BAD_CREDENTIALS:
			log("(App:%p) ++++ CONNECT_EVENT ++++ USER_LOGIN_FAIL_BAD_CREDENTIALS\n",context);
			break;

		case DZ_CONNECT_EVENT_USER_LOGIN_FAIL_USER_INFO:
			log("(App:%p) ++++ CONNECT_EVENT ++++ USER_LOGIN_FAIL_USER_INFO\n",context);
			break;

		case DZ_CONNECT_EVENT_USER_LOGIN_FAIL_OFFLINE_MODE:
			log("(App:%p) ++++ CONNECT_EVENT ++++ USER_LOGIN_FAIL_OFFLINE_MODE\n",context);
			break;

		case DZ_CONNECT_EVENT_ADVERTISEMENT_START:
			log("(App:%p) ++++ CONNECT_EVENT ++++ ADVERTISEMENT_START\n",context);
			break;

		case DZ_CONNECT_EVENT_ADVERTISEMENT_STOP:
			log("(App:%p) ++++ CONNECT_EVENT ++++ ADVERTISEMENT_STOP\n",context);
			break;

		case DZ_CONNECT_EVENT_UNKNOWN:
		default:
			log("(App:%p) ++++ CONNECT_EVENT ++++ UNKNOWN or default (type = %d)\n",context,type);
			break;
	}
}

static void app_change_content(char * content) {

	if (app_ctxt->sz_content_url) {
		free(app_ctxt->sz_content_url);
	}
	app_ctxt->sz_content_url = (char*)calloc(1,strlen(content)+1);
	memccpy(app_ctxt->sz_content_url, content, 1, strlen(content));

	log("CHANGE => %s\n", app_ctxt->sz_content_url);
}

static void app_play_afterload(
		void* delegate,
		void* operation_userdata,
		dz_error_t status,
		dz_object_handle result)
{
	log("PLAY ALBUM=> %s\n", app_ctxt->sz_content_url);
	dz_player_stop(app_ctxt->dzplayer, NULL, NULL);
	dz_player_play(app_ctxt->dzplayer, NULL, NULL,DZ_PLAYER_PLAY_CMD_START_TRACKLIST,DZ_INDEX_IN_QUEUELIST_CURRENT);
}

static void app_play_afterload_album(
		void* delegate,
		void* operation_userdata,
		dz_error_t status,
		dz_object_handle result)
{
#ifdef LOG_DEEZER_ENABLE
	log("PLAY ALBUM TRACK\n");
#endif
	//dz_player_stop(app_ctxt->dzplayer, NULL, NULL);
	dz_player_play(app_ctxt->dzplayer, NULL, NULL,DZ_PLAYER_PLAY_CMD_START_TRACKLIST,DZ_INDEX_IN_QUEUELIST_CURRENT);
}

static void app_play_afterload_playlist(
		void* delegate,
		void* operation_userdata,
		dz_error_t status,
		dz_object_handle result)
{
	log("PLAY PLAYLIST TRACK\n");
	//dz_player_stop(app_ctxt->dzplayer, NULL, NULL);
	dz_player_play(app_ctxt->dzplayer, NULL, NULL,DZ_PLAYER_PLAY_CMD_START_TRACKLIST,DZ_INDEX_IN_QUEUELIST_CURRENT);
}

static void app_load_content() {
	log("LOAD => %s\n", app_ctxt->sz_content_url);

	system("killall -q parec lame");

	app_ctxt->is_playing_album=false;
	app_ctxt->is_playing_playlist=false;

	dz_player_load(app_ctxt->dzplayer,&app_play_afterload,NULL,app_ctxt->sz_content_url);


}

static void app_load_album(peeAlbum* pAlbum) {
	char szTemp[512];

	system("killall -q parec lame");

	app_ctxt->is_playing_album=true;
	app_ctxt->is_playing_playlist=false;

	app_ctxt->pAlbum=pAlbum;
	app_ctxt->pPlaylist=NULL;

	sprintf(szTemp,"dzmedia:///track/%s",pAlbum->_tracks->at(pAlbum->GetFirstMissingTrack())->_id);
#ifdef 	LOG_DEEZER_ENABLE
	log("LOAD ALBUM TRACKS=> %s\n", szTemp);
#endif
	dz_player_load(app_ctxt->dzplayer,&app_play_afterload_album,NULL,szTemp);
}

static void app_load_playlist(peePlaylist* pPlaylist) {
	char szTemp[512];

	system("killall -q parec lame");

	app_ctxt->is_playing_album=false;
	app_ctxt->is_playing_playlist=true;

	app_ctxt->pAlbum=NULL;
	app_ctxt->pPlaylist=pPlaylist;

	sprintf(szTemp,"dzmedia:///track/%s",pPlaylist->_tracks->at(pPlaylist->GetFirstTrack())->_id);
	log("LOAD PLAYLIST TRACKS=> %s\n", szTemp);
	dz_player_load(app_ctxt->dzplayer,&app_play_afterload_playlist,NULL,szTemp);
}

static void app_load_album_next() {
	char szTemp[512];

	app_ctxt->pAlbum->GetNextMissingTrack();
	if(app_ctxt->pAlbum->_currentTrack<app_ctxt->pAlbum->_tracks->size())
	{
		sprintf(szTemp,"dzmedia:///track/%s",app_ctxt->pAlbum->_tracks->at(app_ctxt->pAlbum->_currentTrack)->_id);
		log("LOAD ALBUM TRACKS=> %s\n", szTemp);
		dz_player_load(app_ctxt->dzplayer,&app_play_afterload_album,NULL,szTemp);
	}
	else
	{
		app_ctxt->is_playing=false;
	}
}

static void app_load_playlist_next() {
	char szTemp[512];

	app_ctxt->pPlaylist->GetNextTrack();
	if(app_ctxt->pPlaylist->_currentTrack<app_ctxt->pPlaylist->_tracks->size())
	{
		sprintf(szTemp,"dzmedia:///track/%s",app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_id);
		log("LOAD PLAYLIST TRACKS=> %s\n", szTemp);
		dz_player_load(app_ctxt->dzplayer,&app_play_afterload_playlist,NULL,szTemp);
	}
	else
		{
			app_ctxt->is_playing=false;
		}
}

static void app_playback_start_or_stop() {

	if (!app_ctxt->is_playing) {
		//log("PLAY track n° %d of => %s\n", app_ctxt->nb_track_played, app_ctxt->sz_content_url);
		dz_player_play(app_ctxt->dzplayer, NULL, NULL,
				DZ_PLAYER_PLAY_CMD_START_TRACKLIST,
				DZ_INDEX_IN_QUEUELIST_CURRENT);

	} else {
		log("STOP => %s\n", app_ctxt->sz_content_url);
		dz_player_stop(app_ctxt->dzplayer, NULL, NULL);
	}
}

static void app_playback_start() {

	if (!app_ctxt->is_playing) {
		//log("PLAY track n° %d of => %s\n", app_ctxt->nb_track_played, app_ctxt->sz_content_url);
		dz_player_play(app_ctxt->dzplayer, NULL, NULL,
				DZ_PLAYER_PLAY_CMD_START_TRACKLIST,
				DZ_INDEX_IN_QUEUELIST_CURRENT);

	}
}

static void app_playback_stop() {

	if (app_ctxt->is_playing) {
		log("STOP => %s\n", app_ctxt->sz_content_url);
		dz_player_stop(app_ctxt->dzplayer, NULL, NULL);

	}
}

static void app_playback_pause_or_resume() {

	if (app_ctxt->is_playing) {
		log("PAUSE track n° %d of => %s\n", app_ctxt->nb_track_played, app_ctxt->sz_content_url);
		dz_player_pause(app_ctxt->dzplayer, NULL, NULL);
		app_ctxt->is_playing=false;
	} else {
		log("RESUME track n° %d of => %s\n", app_ctxt->nb_track_played, app_ctxt->sz_content_url);
		dz_player_resume(app_ctxt->dzplayer, NULL, NULL);
		app_ctxt->is_playing=true;
	}
}

static void app_playback_pause() {

	if (app_ctxt->is_playing) {
		log("PAUSE track n° %d of => %s\n", app_ctxt->nb_track_played, app_ctxt->sz_content_url);
		dz_player_pause(app_ctxt->dzplayer, NULL, NULL);
		app_ctxt->is_playing=false;
	}
}

static void app_playback_resume() {

	if (!app_ctxt->is_playing) {
		log("RESUME track n° %d of => %s\n", app_ctxt->nb_track_played, app_ctxt->sz_content_url);
		dz_player_resume(app_ctxt->dzplayer, NULL, NULL);
		app_ctxt->is_playing=true;
	}
}

static void app_playback_toogle_repeat() {


	//TODO removed app_ctxt->repeat_mode++;
	if (app_ctxt->repeat_mode > DZ_QUEUELIST_REPEAT_MODE_ALL) {
		app_ctxt->repeat_mode = DZ_QUEUELIST_REPEAT_MODE_OFF;
	}

	log("REPEAT mode => %d\n", app_ctxt->repeat_mode);

	dz_player_set_repeat_mode(app_ctxt->dzplayer,
			NULL,
			NULL,
			app_ctxt->repeat_mode);
	return;
}

static void app_playback_random_on() {

	app_ctxt->is_shuffle_mode = (app_ctxt->is_shuffle_mode?false:true);

	log("SHUFFLE mode => %s\n", app_ctxt->is_shuffle_mode?"ON":"OFF");

	dz_player_enable_shuffle_mode(app_ctxt->dzplayer,
			NULL,
			NULL,
			1);
	app_ctxt->is_shuffle_mode=true;
}

static void app_playback_random_off() {

	app_ctxt->is_shuffle_mode = (app_ctxt->is_shuffle_mode?false:true);

	log("SHUFFLE mode => %s\n", app_ctxt->is_shuffle_mode?"ON":"OFF");

	dz_player_enable_shuffle_mode(app_ctxt->dzplayer,
			NULL,
			NULL,
			0);
	app_ctxt->is_shuffle_mode=false;
}

static void app_playback_next() {

	log("NEXT => %s\n", app_ctxt->sz_content_url);
	dz_player_play(app_ctxt->dzplayer,
			NULL,
			NULL,
			DZ_PLAYER_PLAY_CMD_START_TRACKLIST,
			DZ_INDEX_IN_QUEUELIST_NEXT);
}

static void app_playback_previous() {
	log("PREVIOUS => %s\n", app_ctxt->sz_content_url);
	dz_player_play(app_ctxt->dzplayer,
			NULL,
			NULL,
			DZ_PLAYER_PLAY_CMD_START_TRACKLIST,
			DZ_INDEX_IN_QUEUELIST_PREVIOUS);

}

void app_player_onevent_cb( dz_player_handle       handle,
		dz_player_event_handle event,
		void *                 supervisor) {


	dz_streaming_mode_t   streaming_mode;
	dz_index_in_queuelist idx;
	app_context_handle context = (app_context_handle)supervisor;

	dz_player_event_t type = dz_player_event_get_type(event);

	if (!dz_player_event_get_queuelist_context(event, &streaming_mode, &idx)) {
		streaming_mode = DZ_STREAMING_MODE_ONDEMAND;
		idx = DZ_INDEX_IN_QUEUELIST_INVALID;
	}

	switch (type) {

		case DZ_PLAYER_EVENT_LIMITATION_FORCED_PAUSE:
			log("(App:%p) ==== PLAYER_EVENT ==== LIMITATION_FORCED_PAUSE for idx: %d\n", context, idx);
			break;

		case DZ_PLAYER_EVENT_QUEUELIST_LOADED:
#ifdef LOG_DEEZER_ENABLE
			log("(App:%p) ==== PLAYER_EVENT ==== QUEUELIST_LOADED for idx: %d\n", context, idx);
#endif
			app_playback_start();
			break;

		case DZ_PLAYER_EVENT_QUEUELIST_NO_RIGHT:
			log("(App:%p) ==== PLAYER_EVENT ==== QUEUELIST_NO_RIGHT for idx: %d\n", context, idx);
			system("killall -q parec lame");


						if(app_ctxt->is_playing_album)
						{
							app_load_album_next();
						}

						if(app_ctxt->is_playing_playlist)
						{
							app_load_playlist_next();
						}
			break;

		case DZ_PLAYER_EVENT_QUEUELIST_NEED_NATURAL_NEXT:
#ifdef LOG_DEEZER_ENABLE
			log("(App:%p) ==== PLAYER_EVENT ==== QUEUELIST_NEED_NATURAL_NEXT for idx: %d\n", context, idx);
#endif
			break;

		case DZ_PLAYER_EVENT_QUEUELIST_TRACK_NOT_AVAILABLE_OFFLINE:
			log("(App:%p) ==== PLAYER_EVENT ==== QUEUELIST_TRACK_NOT_AVAILABLE_OFFLINE for idx: %d\n", context, idx);
			break;

		case DZ_PLAYER_EVENT_QUEUELIST_TRACK_RIGHTS_AFTER_AUDIOADS:
			log("(App:%p) ==== PLAYER_EVENT ==== QUEUELIST_TRACK_RIGHTS_AFTER_AUDIOADS for idx: %d\n", context, idx);
			dz_player_play_audioads(app_ctxt->dzplayer, NULL, NULL);
			break;

		case DZ_PLAYER_EVENT_QUEUELIST_SKIP_NO_RIGHT:
			log("(App:%p) ==== PLAYER_EVENT ==== QUEUELIST_SKIP_NO_RIGHT for idx: %d\n", context, idx);
			break;

		case DZ_PLAYER_EVENT_QUEUELIST_TRACK_SELECTED:
		{
			bool is_preview;
			bool can_pause_unpause;
			bool can_seek;
			int  nb_skip_allowed;
			const char *selected_dzapiinfo;
			const char *next_dzapiinfo;

			is_preview = dz_player_event_track_selected_is_preview(event);

			dz_player_event_track_selected_rights(event, &can_pause_unpause, &can_seek, &nb_skip_allowed);

			selected_dzapiinfo = dz_player_event_track_selected_dzapiinfo(event);
			next_dzapiinfo = dz_player_event_track_selected_next_track_dzapiinfo(event);

			/*
			log("(App:%p) ==== PLAYER_EVENT ==== QUEUELIST_TRACK_SELECTED for idx: %d - is_preview:%d\n", context, idx, is_preview);
			log("\tcan_pause_unpause:%d can_seek:%d nb_skip_allowed:%d\n", can_pause_unpause, can_seek, nb_skip_allowed);
			if (selected_dzapiinfo)
				//log("\tnow playing:%s\n", selected_dzapiinfo);
				log("\tnow playin!:\n");
			if (next_dzapiinfo)
				log("\tnext:%s\n", next_dzapiinfo);
			*/
		}

		app_ctxt->nb_track_played++;
		break;

		case DZ_PLAYER_EVENT_MEDIASTREAM_DATA_READY:
#ifdef 	LOG_DEEZER_ENABLE
			log("(App:%p) ==== PLAYER_EVENT ==== MEDIASTREAM_DATA_READY for idx: %d\n", context, idx);
#endif
			break;

		case DZ_PLAYER_EVENT_MEDIASTREAM_DATA_READY_AFTER_SEEK:
			log("(App:%p) ==== PLAYER_EVENT ==== MEDIASTREAM_DATA_READY_AFTER_SEEK for idx: %d\n", context, idx);
			break;

		case DZ_PLAYER_EVENT_RENDER_TRACK_START_FAILURE:

			log("(App:%p) ==== PLAYER_EVENT ==== RENDER_TRACK_START_FAILURE for idx: %d\n", context, idx);

			app_ctxt->is_playing = false;
			system("killall -q parec lame");


			if(app_ctxt->is_playing_album)
			{
				app_load_album_next();
			}

			if(app_ctxt->is_playing_playlist)
			{
				app_load_playlist_next();
			}

			break;

		case DZ_PLAYER_EVENT_RENDER_TRACK_START:
#ifdef 	LOG_DEEZER_ENABLE
			log("(App:%p) ==== PLAYER_EVENT ==== RENDER_TRACK_START for idx: %d\n", context, idx);
#endif
			app_ctxt->is_underflow=false;
			app_ctxt->is_playing = true;
			if(app_ctxt->is_playing_album)
			{
				sem_post(&semaphorRecord);
				log("START: %s %s %s\n",app_ctxt->pAlbum->_artisteName,app_ctxt->pAlbum->_albumName,
										app_ctxt->pAlbum->_tracks->at(app_ctxt->pAlbum->_currentTrack)->_title);
			}
			if(app_ctxt->is_playing_playlist)
			{
				sem_post(&semaphorRecord);
				log("START: %s %s %s\n",app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_title,
										app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_szAlbum,
										app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_szArtist);
			}
			break;

		case DZ_PLAYER_EVENT_RENDER_TRACK_END:
#ifdef LOG_DEEZER_ENABLE
			log("(App:%p) ==== PLAYER_EVENT ==== RENDER_TRACK_END for idx: %d\n", context, idx);
#endif
			app_ctxt->is_playing = false;
			log("- nb_track_played : %d\n",app_ctxt->nb_track_played);

			// Detect if we come from from playing an ad, if yes restart automatically the playback.
			if (idx == DZ_INDEX_IN_QUEUELIST_INVALID) {
				app_playback_start_or_stop();
			}

			if(app_ctxt->is_playing_album)
			{
				log("END: %s %s %s\n",app_ctxt->pAlbum->_artisteName,app_ctxt->pAlbum->_albumName,app_ctxt->pAlbum->_tracks->at(app_ctxt->pAlbum->_currentTrack)->_title);
				system("killall -q parec lame");
				char szcmd[512];

				sprintf(szcmd,"mkdir -p \"%s\"",app_ctxt->pAlbum->_tracks->at(app_ctxt->pAlbum->_currentTrack)->_localDir);
				printf("%s\n",szcmd);
				system(szcmd);

				if(app_ctxt->is_underflow==false)
				{
					if(!app_ctxt->pAlbum->_tracks->at(app_ctxt->pAlbum->_currentTrack)->checkDownload())
					{
						sprintf(szcmd,"mv temp.mp3 \"%s\"",app_ctxt->pAlbum->_tracks->at(app_ctxt->pAlbum->_currentTrack)->_localPath);
						printf("%s\n",szcmd);
						system(szcmd);
					}
				}
				app_load_album_next();
			}

			if(app_ctxt->is_playing_playlist)
			{
				log("END: %s %s %s\n",app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_title,
						app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_szAlbum,
						app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_szArtist);
				system("killall -q parec lame");
				char szcmd[512];

				sprintf(szcmd,"mkdir -p \"%s\"",app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_localDir);
				printf("%s\n",szcmd);
				system(szcmd);
				if(app_ctxt->is_underflow==false)
				{
					if(!app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->checkDownload())
					{
						sprintf(szcmd,"mv temp.mp3 \"%s\"",app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_localPath);
						printf("%s\n",szcmd);
						system(szcmd);
					}
				}
				app_load_playlist_next();
			}

			break;

		case DZ_PLAYER_EVENT_RENDER_TRACK_PAUSED:
			log("(App:%p) ==== PLAYER_EVENT ==== RENDER_TRACK_PAUSED for idx: %d\n", context, idx);
			app_ctxt->is_playing = false;
			break;

		case DZ_PLAYER_EVENT_RENDER_TRACK_UNDERFLOW:
			log("(App:%p) ==== PLAYER_EVENT ==== RENDER_TRACK_UNDERFLOW for idx: %d\n", context, idx);
			app_ctxt->is_playing = false;
			app_ctxt->is_underflow=true;
			break;

		case DZ_PLAYER_EVENT_RENDER_TRACK_RESUMED:
			log("(App:%p) ==== PLAYER_EVENT ==== RENDER_TRACK_RESUMED for idx: %d\n", context, idx);
			app_ctxt->is_playing = true;
			break;

		case DZ_PLAYER_EVENT_RENDER_TRACK_SEEKING:
			log("(App:%p) ==== PLAYER_EVENT ==== RENDER_TRACK_SEEKING for idx: %d\n", context, idx);
			app_ctxt->is_playing = false;
			break;

		case DZ_PLAYER_EVENT_RENDER_TRACK_REMOVED:
#ifdef 	LOG_DEEZER_ENABLE
			log("(App:%p) ==== PLAYER_EVENT ==== RENDER_TRACK_REMOVED for idx: %d\n", context, idx);
#endif
			app_ctxt->is_playing = false;
			system("killall -q parec lame");
			break;

		case DZ_PLAYER_EVENT_UNKNOWN:
		default:
			log("(App:%p) ==== PLAYER_EVENT ==== UNKNOWN or default (type = %d)\n", context,type);
			break;
	}
}





void* mainRecord(void* voidtoken) {

	char szCmd[512];
	char szAlbum[256];
	char szArtist[256];
	char szTilte[256];
	char szCoverPath[512];

	while(1)
	{
		/*
		  --tt <title>    audio/song title (max 30 chars for version 1 tag)
		  --ta <artist>   audio/song artist (max 30 chars for version 1 tag)
		  --tl <album>    audio/song album (max 30 chars for version 1 tag)
		  --ty <year>     audio/song year of issue (1 to 9999)
		  --tc <comment>  user-defined text (max 30 chars for v1 tag, 28 for v1.1)
		  --tn <track[/total]>   audio/song track number and (optionally) the total
		 */
		sem_wait(&semaphorRecord);

		if(app_ctxt->is_playing_album)
		{
			sprintf(szAlbum,"%s",app_ctxt->pAlbum->_albumName);
			sprintf(szArtist,"%s",app_ctxt->pAlbum->_artisteName);
			sprintf(szTilte,"%s",app_ctxt->pAlbum->_tracks->at(app_ctxt->pAlbum->_currentTrack)->_title);
			sprintf(szCoverPath,"%s",app_ctxt->pAlbum->_localPathCover);

			cleanMP3String(szAlbum);
			cleanMP3String(szArtist);
			cleanMP3String(szTilte);
		}
		if(app_ctxt->is_playing_playlist)
		{
			sprintf(szAlbum,"%s",app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_szAlbum);
			sprintf(szArtist,"%s",app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_szArtist);
			sprintf(szTilte,"%s",app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_title);
			sprintf(szCoverPath,"%s",app_ctxt->pPlaylist->_tracks->at(app_ctxt->pPlaylist->_currentTrack)->_pAlbum->_localPathCover);

			cleanMP3String(szAlbum);
			cleanMP3String(szArtist);
			cleanMP3String(szTilte);
		}
		//sprintf(szCmd,"parec --format=s16le -d record-n-play.monitor |   lame -r --quiet --ti \"%s\" -q 3 --lowpass 17 --abr 192 - \"temp.mp3\" "
		sprintf(szCmd,"parec --format=s16le   |   lame -r --quiet --ti \"%s\" -q 3 --lowpass 17 --abr 192 - \"temp.mp3\" "
				"--tt \"%s\" "
				"--ta \"%s\" "
				"--tl \"%s\" ",
				szCoverPath,
				szTilte,
				szArtist,
				szAlbum);
		printf(szCmd);
		log("Start record\n");
		system(szCmd);
		log("End record\n");
	}
}


static void dz_connect_on_deactivate(void*            delegate,
		void*            operation_userdata,
		dz_error_t       status,
		dz_object_handle result) {
	app_ctxt->activation_count--;
	log("CONNECT deactivated - c = %d with status = %d\n",
			app_ctxt->activation_count,
			status);
}

static void dz_player_on_deactivate(void*            delegate,
		void*            operation_userdata,
		dz_error_t       status,
		dz_object_handle result) {
	app_ctxt->activation_count--;
	log("PLAYER deactivated - c = %d with status = %d\n",
			app_ctxt->activation_count,
			status);
}

static void app_commands_get_next() {

	char strBuf[1024];
	int cmd;
	const void* arg;
	const char* name;
	sem_wait(&semaphorDeezer);

	cmd=deezerCmd[deezerCmdIndexRd].Cmd;
	arg=deezerCmd[deezerCmdIndexRd].Arg;
	name=deezerCmd[deezerCmdIndexRd].NameToDisplay;
	deezerCmdIndexRd++;
	if(deezerCmdIndexRd>15)
		deezerCmdIndexRd=0;


	printf("Deezer cmd%i arg%i\n",cmd,arg);
	switch (cmd) {
		case DEEZER_CMD_START:
			app_playback_start();
			break;

		case DEEZER_CMD_STOP:
			app_playback_stop();
			break;

		case DEEZER_CMD_RESUME:
			app_playback_resume();
			break;

		case DEEZER_CMD_PAUSE:
			app_playback_pause_or_resume();
			if(mpcPause)
			{
				sprintf(strBuf,"mpc play",arg);
				system(strBuf);//launch mp3 player
				mpcPause=0;
			}
			else
			{
				sprintf(strBuf,"mpc pause",arg);
				system(strBuf);//launch mp3 player
				mpcPause=1;
			}
			break;

		case DEEZER_CMD_NEXT:
			if(app_ctxt->is_playing_album)
				app_load_album_next();
			else if(app_ctxt->is_playing_playlist)
				app_load_playlist_next();
			else
				app_playback_next();

			system("mpc next");
			break;

		case DEEZER_CMD_PREV:
			app_playback_previous();
			system("mpc prev");
			break;

		case DEEZER_CMD_REPEAT_MODE_OFF:
			app_playback_toogle_repeat();
			break;

		case DEEZER_CMD_RANDOM_ON:
			app_playback_random_on();
			break;

		case DEEZER_CMD_RANDOM_OFF:
			app_playback_random_off();

			break;
		case DEEZER_CMD_EXIT:
			is_shutting_down = true;
			app_shutdown();
			break;

		case DEEZER_CMD_LOAD_ALBUM:
			system("mpc clear");
			app_load_album((peeAlbum*)arg);
			szCurrentSong=NULL;
			break;

		case DEEZER_CMD_LOAD_PLAYLIST:
			/*
			system("mpc clear");
			sprintf(strBuf,"dzmedia:///playlist/%s",arg);
			app_change_content(strBuf);
			app_load_content();
			szCurrentSong=name;
			*/
			system("mpc clear");
			app_load_playlist((peePlaylist*)arg);
			szCurrentSong=NULL;

			break;

		case DEEZER_CMD_LOAD_PODCAST_MP3:
			app_playback_stop();
			mpcPause=0;
			sprintf(strBuf,"mpc clear;mpc add \"%s\";mpc play",arg);
			printf("%s\n",strBuf);
			system(strBuf);//launch mp3 player
			szCurrentSong=name;
			break;

		case DEEZER_CMD_LOAD_DIR_MP3:
			app_playback_stop();
			mpcPause=0;
			sprintf(strBuf,"mpc clear;mpc add mp3/\"%s\";mpc play",arg);
			printf("%s\n",strBuf);
			system(strBuf);//launch mp3 player
			szCurrentSong=name;
			break;


	}
}

int deezerLaunch(char* token)
{
	pthread_t my_thread;
	pthread_t my_thread_recoreder;
	int ret;

	system("killall -q parec lame");

	sem_init(&semaphorDeezer,0,0);
	sem_init(&semaphorRecord,0,0);

	ret =  pthread_create(&my_thread, NULL, &mainDeezer, token);
	if(ret != 0) {
		printf("Error: pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}

	ret =  pthread_create(&my_thread_recoreder, NULL, &mainRecord, token);
	if(ret != 0) {
		printf("Error: pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}

	//pthread_exit(NULL);
	return 0;
}

int deezerPostCommand(uint32_t cmd,const void* arg,const char* name )
{

	deezerCmd[deezerCmdIndexWr].Cmd=cmd;
	deezerCmd[deezerCmdIndexWr].Arg=arg;
	deezerCmd[deezerCmdIndexWr].NameToDisplay=name;

	deezerCmdIndexWr++;
	if(deezerCmdIndexWr>15)
		deezerCmdIndexWr=0;

	sem_post(&semaphorDeezer);
}

const char* deezerGetSongName()
{
	return szCurrentSong;
}

bool deezerIsPlaying()
{
	if(app_ctxt->is_playing_album)
	{
		if(app_ctxt->pAlbum->_currentTrack<app_ctxt->pAlbum->GetNbrTracks())
			return true;
	}
	if(app_ctxt->is_playing_playlist)
	{
		if(app_ctxt->pPlaylist->_currentTrack<app_ctxt->pPlaylist->GetNbrTracks())
			return true;
	}
	return  false;
}


bool deezerIsReady()
{
	if(app_ctxt==NULL)
		return false;

	return app_ctxt->is_ready;
}
