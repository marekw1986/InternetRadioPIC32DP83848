/* 
 * File:   vs1053.h
 * Author: Przemyslaw Stasiak
 *
 * Created on 4 maja 2020, 10:33
 */

#ifndef VS1053_H
#define	VS1053_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

#define RXNE    0x01
#define TXE     0x02
#define BSY     0x80

typedef enum {
    STREAM_HOME = 0,
    STREAM_HTTP_BEGIN,
    STREAM_HTTP_NAME_RESOLVE,
    STREAM_HTTP_OBTAIN_SOCKET,            
    STREAM_HTTP_SOCKET_OBTAINED,
    STREAM_HTTP_SEND_REQUEST,
    STREAM_HTTP_PROCESS_HEADER,
    STREAM_HTTP_FILL_BUFFER,
    STREAM_HTTP_GET_DATA,
    STREAM_FILE_FILL_BUFFER,   
    STREAM_FILE_PLAY_REST,
    STREAM_FILE_GET_DATA,
    STREAM_HTTP_CLOSE,
    STREAM_HTTP_RECONNECT_WAIT        
} StreamState_t;

enum {VS_MSG_NEXT, VS_MSG_PREV, VS_MSG_STOP, VS_MSG_PLAY_FILE, VS_MSG_PLAY_DIR, VS_MSG_PLAY_STREAM_BY_ID, VS_MSG_SET_VOL, VS_MSG_LOOP};

typedef struct {
	uint16_t cmd;
    uint16_t reserved;
	uint32_t param;
} vs1053cmd_t;

typedef struct {
    const char* name;
    const char* url;
} radio_t;

#ifdef	__cplusplus
extern "C" {
#endif


//public functions
void VS1053_init(void);    
void VS1053_handle (void);
void VS1053_setVolume(uint8_t vol);
uint8_t VS1053_getVolume(void);
void VS1053_play_next(void);
void VS1053_play_prev(void);
void VS1053_play_next_audio_file_from_directory (void);
void VS1053_play_http_stream(const char* url);
bool VS1053_play_http_stream_by_id(uint16_t id);
void VS1053_play_next_http_stream_from_list(void);
void VS1053_play_prev_http_stream_from_list(void);
void VS1053_play_file (char* url);
void VS1053_play_dir (const char* url);
void VS1053_stop(void);
void VS1053_setLoop(bool val);
bool VS1053_getLoop(void);
StreamState_t VS1053_getStreamState(void);
uint8_t is_audio_file (char* name);
void VS1053_activate_spectrum_analyzer(void);
void VS1053_read_spectrum_analyzer(void);
void VS1053_send_cmd_thread_safe(uint8_t cmd, uint32_t param);
const char* VS1053_get_state_description(void);
void VS1053B_apply_patch(void);


#ifdef	__cplusplus
}
#endif

#endif	/* VS1053_H */
