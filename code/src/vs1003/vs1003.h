/* 
 * File:   vs1003.h
 * Author: Przemyslaw Stasiak
 *
 * Created on 4 maja 2020, 10:33
 */

#ifndef VS1003_H
#define	VS1003_H

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

enum {VS_MSG_NEXT, VS_MSG_STOP, VS_MSG_PLAY_FILE, VS_MSG_PLAY_DIR, VS_MSG_PLAY_STREAM_BY_ID, VS_MSG_SET_VOL, VS_MSG_LOOP};

typedef struct {
	uint16_t cmd;
    uint16_t reserved;
	uint32_t param;
} vs1003cmd_t;

typedef struct {
    const char* name;
    const char* url;
} radio_t;

#ifdef	__cplusplus
extern "C" {
#endif


//public functions
void VS1003_begin(void);    
uint16_t VS1003_read_register(uint8_t _reg);
void VS1003_write_register(uint8_t _reg,uint16_t _value);
void VS1003_sdi_send_buffer(const uint8_t* data, int len);
void VS1003_sdi_send_zeroes(int len);
void VS1003_handle (void);
void VS1003_setVolume(uint8_t vol);
uint8_t VS1003_getVolume(void);
void VS1003_playChunk(const uint8_t* data, size_t len);
void VS1003_print_byte_register(uint8_t reg);
void VS1003_printDetails(void);
void VS1003_loadUserCode(const uint16_t* buf, size_t len);
void VS1003_play_next(void);
void VS1003_play_next_audio_file_from_directory (void);
void VS1003_play_http_stream(const char* url);
void VS1003_play_http_stream_by_id(uint16_t id);
void VS1003_play_next_http_stream_from_list(void);
void VS1003_play_file (char* url);
void VS1003_play_dir (const char* url);
void VS1003_stop(void);
void VS1003_setLoop(bool val);
bool VS1003_getLoop(void);
StreamState_t VS1003_getStreamState(void);
uint8_t is_audio_file (char* name);
void VS1003_activate_spectrum_analyzer(void);
void VS1003_read_spectrum_analyzer(void);
void VS1003_send_cmd_thread_safe(uint8_t cmd, uint32_t param);


#ifdef	__cplusplus
}
#endif

#endif	/* VS1003_H */
