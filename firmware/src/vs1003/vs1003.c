/*
 Copyright (C) 2023 Marek Wiecek
 High level driver for VS1003 chip, designed to operate with
 low level written by Andy Karpov.
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
 
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <tcpip/tcpip.h>
#include <tcpip/dns.h>

#include "app.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "system/debug/sys_debug.h"
#include "system/fs/sys_fs.h"
#include "vs1003.h"
#include "vs1003_low_level.h"
#include "mediainfo.h"
#include "ringbuffer.h"
#include "../common.h"
#ifdef USE_LCD_UI
#include "../lcd/ui.h"
#endif

#define RECONNECT_LIMIT 3

QueueHandle_t vsQueueHandle;

static SYS_FS_HANDLE fsrc;
static SYS_FS_HANDLE vsdir;
static uint8_t ReconnectLimit = RECONNECT_LIMIT;
static int current_stream_ind = 1;

static TCP_SOCKET VS_Socket = INVALID_SOCKET;
static uri_t uri;
static bool loop_flag = false;
static bool dir_flag = false;
static uint8_t last_volume = 0;

static StreamState_t StreamState = STREAM_HOME;

typedef enum {
    DO_NOT_RECONNECT = 0,
    RECONNECT_IMMEDIATELY,
    RECONNECT_WAIT_LONG,
    RECONNECT_WAIT_SHORT
} ReconnectStrategy_t;

static ReconnectStrategy_t ReconnectStrategy = DO_NOT_RECONNECT;

static void VS1003_startPlaying(void);
static void VS1003_stopPlaying(void);
static void VS1003_soft_stop (void);
static void VS1003_handle_end_of_file (void);

void VS1003_init(void) {
    VS1003_low_level_init();
    vsQueueHandle = xQueueCreate(16, sizeof(vs1003cmd_t));
    if (vsQueueHandle == NULL) {
        SYS_CONSOLE_PRINT("ERROR: Can't create VS queue\r\n");
        while(1);
    }
}

void VS1003_handle(void) {   
	static uint32_t	timer;
    static IP_MULTI_ADDRESS ServerAddress;
    TCPIP_DNS_RESULT dnsres;
    int w; // to_load;
    uint8_t data[32];
    int br;
    vs1003cmd_t rcv;
    //SYS_FS_RESULT fres;
    
	switch(StreamState)
	{
		case STREAM_HOME:
            //nothing to do here, just wait
            break;
        
        case STREAM_HTTP_BEGIN:            
            // Obtain the IP address associated with the radio server
            if(strlen(uri.server)) {
                dnsres = TCPIP_DNS_Resolve(uri.server, TCPIP_DNS_TYPE_A);
                if (dnsres < 0) {
                    StreamState = STREAM_HOME;
                    #ifdef USE_LCD_UI
                    ui_update_state_info(NULL);
                    #endif
                    SYS_CONSOLE_PRINT("TCPIP_DNS_Resolve error %d\r\n", dnsres);
                    break;
                }
                else if (dnsres == TCPIP_DNS_RES_NAME_IS_IPADDRESS) {
                    //TODO
                    TCPIP_Helper_StringToIPAddress(uri.server, &(ServerAddress.v4Add));
                    StreamState = STREAM_HTTP_OBTAIN_SOCKET;
                    break;
                }
                else {
                    StreamState = STREAM_HTTP_NAME_RESOLVE;
                    SYS_CONSOLE_PRINT("DNS begin: %s\r\n", uri.server);
                    clear_ringbuffer();
                    break;
                }
            }
            else {
                StreamState = STREAM_HOME;
                #ifdef USE_LCD_UI
                ui_update_state_info(NULL);
                #endif                
                SYS_CONSOLE_PRINT("DNS uri.server empty\r\n");
                break;
            }
            SYS_CONSOLE_PRINT("TCPIP_DNS_Resolve: Something went wrong\r\n");
			break;
            
        case STREAM_HTTP_NAME_RESOLVE:;
            // Wait for the DNS server to return the requested IP address
            dnsres = TCPIP_DNS_IsResolved(uri.server, &ServerAddress, TCPIP_DNS_TYPE_A);
            if (dnsres < 0) {
                StreamState = STREAM_HOME;
                #ifdef USE_LCD_UI
                ui_update_state_info(NULL);
                #endif
                SYS_CONSOLE_PRINT("TCPIP_DNS_IsResolved error: %d\r\n", dnsres);
                break;
            }
            else if ( dnsres == TCPIP_DNS_RES_PENDING) {
                break;
            }
            else if ( dnsres == TCPIP_DNS_RES_OK ) {
                StreamState = STREAM_HTTP_OBTAIN_SOCKET;
                SYS_CONSOLE_PRINT("DNS OK\r\n");
                TCPIP_Helper_IPAddressToString(&ServerAddress.v4Add, (char*)data, 32);        
                SYS_CONSOLE_PRINT("IP address: %s\r\n", data);
                SYS_CONSOLE_PRINT("Obtaining socket\r\n");
                TCPIP_DNS_RemoveEntry(uri.server);
                break;
            }
            SYS_CONSOLE_PRINT("TCPIP_DNS_IsResolved: something went wrong\r\n");
            break;
            
        case STREAM_HTTP_OBTAIN_SOCKET:
            VS_Socket = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4, uri.port, &ServerAddress);
            
            if(VS_Socket == INVALID_SOCKET) {
                StreamState=STREAM_HTTP_CLOSE;
                ReconnectStrategy = RECONNECT_WAIT_SHORT;
                #ifdef USE_LCD_UI
                ui_update_state_info("Reconnecting");
                #endif
                SYS_CONSOLE_PRINT("Socket fail\r\n");
				break;
            }
            StreamState = STREAM_HTTP_SOCKET_OBTAINED;
            SYS_CONSOLE_PRINT("Socket obtained\r\n");
            timer = millis();
            break;            

		case STREAM_HTTP_SOCKET_OBTAINED:
			// Wait for the remote server to accept our connection request
			if(!TCPIP_TCP_IsConnected(VS_Socket))
			{
                // Time out if too much time is spent in this state
				if((uint32_t)(millis()-timer) > 5000)
				{
					SYS_CONSOLE_PRINT("Socket timeout\r\n");
                    // Close the socket so it can be used by other modules
					if (!TCPIP_TCP_Disconnect(VS_Socket)) {
                        TCPIP_TCP_Abort(VS_Socket, true);
                    }
					VS_Socket = INVALID_SOCKET;
					StreamState = STREAM_HTTP_CLOSE;     //was StreamState--
                    ReconnectStrategy = DO_NOT_RECONNECT;
                    //TODO limit number of reconnections or repair differently
                    //If we stuck here, it breaks state machine!!!!!
				}
				break;
			}
            SYS_CONSOLE_PRINT("Connected: sending request\r\n");
            StreamState = STREAM_HTTP_SEND_REQUEST;
			timer = millis();
            break;
            
        case STREAM_HTTP_SEND_REQUEST:
			// Make certain the socket can be written to
			if( TCPIP_TCP_PutIsReady(VS_Socket) < (49u + strlen(uri.file) + strlen(uri.server)) ) {
                if ( (uint32_t)(millis()-timer) > 5000 ) {
                    StreamState = STREAM_HTTP_CLOSE;
                    ReconnectStrategy = RECONNECT_WAIT_LONG;
                }
				break;
            }
			
			// Place the application protocol data into the transmit buffer.  For this example, we are connected to an HTTP server, so we'll send an HTTP GET request.
			TCPIP_TCP_StringPut(VS_Socket, (const uint8_t*)"GET ");
			TCPIP_TCP_StringPut(VS_Socket, (const uint8_t*)uri.file);
			TCPIP_TCP_StringPut(VS_Socket, (const uint8_t*)" HTTP/1.0\r\nHost: ");
			TCPIP_TCP_StringPut(VS_Socket, (const uint8_t*)uri.server);
			TCPIP_TCP_StringPut(VS_Socket, (const uint8_t*)"\r\nConnection: keep-alive\r\n\r\n");
//
            SYS_CONSOLE_PRINT("Sending headers\r\n");
            
            TCPIP_TCP_WasReset(VS_Socket);
			// Send the packet
			TCPIP_TCP_Flush(VS_Socket);
            timer = millis();
			StreamState = STREAM_HTTP_PROCESS_HEADER;
            prepare_http_parser();
			break;
            
        case STREAM_HTTP_PROCESS_HEADER:
			if(TCPIP_TCP_WasReset(VS_Socket))
			{
				StreamState = STREAM_HTTP_CLOSE;
                ReconnectStrategy = RECONNECT_IMMEDIATELY;
//                #ifdef USE_LCD_UI
//                lcd_ui_update_state_info("Reconnecting");
//                #endif
                SYS_CONSOLE_PRINT("Internet radio: socket disconnected - reseting\r\n");
				break;
			}
            
            w = TCPIP_TCP_ArrayGet(VS_Socket, data, 32);
            if (w) {
                http_res_t http_result = parse_http_headers((char*)data, w, &uri);
                switch (http_result) {
                    case HTTP_HEADER_ERROR:
                        SYS_CONSOLE_PRINT("Parsing headers error\r\n");
                        prepare_http_parser();
                        ReconnectStrategy = RECONNECT_IMMEDIATELY;
                        StreamState = STREAM_HTTP_CLOSE;
//                        #ifdef USE_LCD_UI
//                        lcd_ui_update_state_info("Reconnecting");
//                        #endif
                        break;
                    case HTTP_HEADER_OK:
                        SYS_CONSOLE_PRINT("It is 200 OK\r\n");
                        timer = millis();
                        //memcpy(&uri, &tempUri, sizeof(uri_t));
                        StreamState = STREAM_HTTP_FILL_BUFFER;     //STREAM_HTTP_GET_DATA
                        VS1003_startPlaying();
                        break;
                    case HTTP_HEADER_REDIRECTED:
                        SYS_CONSOLE_PRINT("Stream redirected\r\n");
//                        /memcpy(&uri, &uri, sizeof(uri_t));
                        ReconnectStrategy = RECONNECT_IMMEDIATELY;
                        StreamState = STREAM_HTTP_CLOSE;
                        break;
                    case HTTP_HEADER_IN_PROGRESS:
                        break;
                    default:
                        break;
                }
            }
            
            if ( (uint32_t)(millis()-timer) > 5000) {
                //There was no data in 2 seconds - reconnect
                SYS_CONSOLE_PRINT("Internet radio: no header timeout - reseting\r\n");
                ReconnectStrategy = RECONNECT_WAIT_SHORT;
                StreamState = STREAM_HTTP_CLOSE;
                #ifdef USE_LCD_UI
                ui_update_state_info("Reconecting");
                #endif
            }            
            break;
            
        case STREAM_HTTP_FILL_BUFFER:
            while (get_remaining_space_in_ringbuffer() > 128) {
                w = TCPIP_TCP_ArrayGet(VS_Socket, data, 32);
                if (w) {
                    timer = millis();
                    write_array_to_ringbuffer(data, w);
                }
                else { break; }
            }
            
            if (get_remaining_space_in_ringbuffer() <= 128) {
                SYS_CONSOLE_PRINT("Buffer filled\r\n");
                timer = millis();
                StreamState = STREAM_HTTP_GET_DATA;
                #ifdef USE_LCD_UI
                ui_update_state_info("Playing stream");
                #endif
                break;
            }
    
			if(TCPIP_TCP_WasReset(VS_Socket))
			{
				StreamState = STREAM_HTTP_CLOSE;
                ReconnectStrategy = RECONNECT_WAIT_LONG;
                #ifdef USE_LCD_UI
                ui_update_state_info("Reconnecting");
                #endif
                SYS_CONSOLE_PRINT("Internet radio: socket disconnected - reseting\r\n");
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}
    
            if ( (uint32_t)(millis()-timer) > 5000) {
                //There was no data in 5 seconds - reconnect
                SYS_CONSOLE_PRINT("Internet radio: no new data timeout - reseting\r\n");
                ReconnectStrategy = RECONNECT_WAIT_LONG;
                StreamState = STREAM_HTTP_CLOSE;
                #ifdef USE_LCD_UI
                ui_update_state_info("Reconnecting");
                #endif
            }                        
            break;

		case STREAM_HTTP_GET_DATA:
            while (get_remaining_space_in_ringbuffer() > 128) {
                w = TCPIP_TCP_ArrayGet(VS_Socket, data, 32);
                if (w) {
                    timer = millis();
                    write_array_to_ringbuffer(data, w);
                }
                if (VS_DREQ_PIN) break;
            }
                
            if (VS1003_feed_from_buffer() == FEED_RET_BUFFER_EMPTY) {
                StreamState = STREAM_HTTP_FILL_BUFFER;
                timer = millis();
                break;
            }
			if(TCPIP_TCP_WasReset(VS_Socket))
			{
				StreamState = STREAM_HTTP_CLOSE;
                ReconnectStrategy = RECONNECT_WAIT_LONG;
                #ifdef USE_LCD_UI
                ui_update_state_info("Reconnecting");
                #endif
                SYS_CONSOLE_PRINT("Internet radio: socket disconnected - reseting\r\n");
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}
            if ( (uint32_t)(millis()-timer) > 5000) {
                //There was no data in 5 seconds - reconnect
                SYS_CONSOLE_PRINT("Internet radio: no new data timeout - reseting\r\n");
                ReconnectStrategy = RECONNECT_WAIT_LONG;
                StreamState = STREAM_HTTP_CLOSE;
                #ifdef USE_LCD_UI
                ui_update_state_info("Reconnecting");
                #endif
            }
			break;
            
         case STREAM_FILE_FILL_BUFFER:
            while (get_remaining_space_in_ringbuffer() > 128) {
                br = SYS_FS_FileRead(fsrc, data, 32);
                if (br != -1) {
                    if (br != 0) { write_array_to_ringbuffer(data, br); }
                    if (br < 32) {  //end of file
                        StreamState = STREAM_FILE_PLAY_REST;
                    }
                }
            }
            if (StreamState == STREAM_FILE_PLAY_REST) break;
            StreamState = STREAM_FILE_GET_DATA;
            #ifdef USE_LCD_UI
            ui_update_state_info("Playing file");
            #endif
            break;
            
        case STREAM_FILE_GET_DATA:
            while (get_remaining_space_in_ringbuffer() > 128) {
                br = SYS_FS_FileRead(fsrc, data, 32);
                if ( br != -1 ) {
                    if (br) {write_array_to_ringbuffer(data, br); }
                    if (br < 32) {     //end of file
                        StreamState = STREAM_FILE_PLAY_REST;
                    }
                    if (VS_DREQ_PIN) break;
                }
            }
            if (StreamState == STREAM_FILE_PLAY_REST) break;
            if (VS1003_feed_from_buffer() == FEED_RET_BUFFER_EMPTY) {
                //buffer empty
                StreamState = STREAM_FILE_FILL_BUFFER;
            }            
            break;
            
        case STREAM_FILE_PLAY_REST:
            if (VS1003_feed_from_buffer() == FEED_RET_BUFFER_EMPTY) {
                //buffer empty
            	VS1003_handle_end_of_file();
            }
        	break;            
	
		case STREAM_HTTP_CLOSE:
			// Close the socket so it can be used by other modules
			// For this application, we wish to stay connected, but this state will still get entered if the remote server decides to disconnect
			if (!TCPIP_TCP_Disconnect(VS_Socket)) {
                TCPIP_TCP_Abort(VS_Socket, true);
            }
			VS_Socket = INVALID_SOCKET;
            VS1003_stopPlaying();
            switch(ReconnectStrategy) {
                case DO_NOT_RECONNECT:
                    StreamState = STREAM_HOME;
                    #ifdef USE_LCD_UI
                    ui_update_state_info(NULL);
                    #endif
                    break;
                case RECONNECT_IMMEDIATELY:
                    ReconnectLimit--;
                    if (ReconnectLimit > 0) {
                        StreamState = STREAM_HTTP_BEGIN;
                    }
                    else {
                        StreamState = STREAM_HOME;
                        SYS_CONSOLE_PRINT("Reconnect limit reached\r\n");
                        #ifdef USE_LCD_UI
                        ui_update_state_info(NULL);
                        #endif
                    }
                    break;
                case RECONNECT_WAIT_LONG:
                case RECONNECT_WAIT_SHORT:
                    ReconnectLimit--;
                    if (ReconnectLimit > 0) {
                        StreamState = STREAM_HTTP_RECONNECT_WAIT;
                    }
                    else {
                        StreamState = STREAM_HOME;
                        SYS_CONSOLE_PRINT("Reconnect limit reached\r\n");
                        #ifdef USE_LCD_UI
                        ui_update_state_info(NULL);
                        #endif
                    }
                    break;
                default:
                    StreamState = STREAM_HOME;
                    break;
            }
            timer = millis();
			break;
            
        case STREAM_HTTP_RECONNECT_WAIT:
            if ( (uint32_t)(millis()-timer) > ((ReconnectStrategy == RECONNECT_WAIT_LONG) ? (5000) : (1000)) ) {
                SYS_CONSOLE_PRINT("Internet radio: reconnecting\r\n");
                StreamState = STREAM_HTTP_BEGIN;
            }
            break;
	}
    
	if (xQueueReceive(vsQueueHandle, &rcv, 5) == pdTRUE) {
		SYS_CONSOLE_PRINT("Received command %d from queque\r\n", rcv.cmd);
		switch(rcv.cmd) {
			case VS_MSG_NEXT:
				VS1003_play_next();
				break;
            case VS_MSG_PREV:
                VS1003_play_prev();
                break;
			case VS_MSG_STOP:
				VS1003_stop();
				break;
			case VS_MSG_PLAY_STREAM_BY_ID:
				VS1003_play_http_stream_by_id(rcv.param);
				break;
			case VS_MSG_PLAY_FILE:;
                char* file = (char*)rcv.param;
                SYS_CONSOLE_PRINT("Playing file %s\r\n", file);
                VS1003_stop();
                VS1003_play_file(file);
				break;
			case VS_MSG_PLAY_DIR:;
                char* dir = (char*)rcv.param;
                SYS_CONSOLE_PRINT("Playing direcotry %s\r\n", dir);
                VS1003_stop();
                VS1003_play_dir(dir);                
				break;
			case VS_MSG_SET_VOL:
				if ( (rcv.param >= 1) && (rcv.param <= 100) ) {
                    VS1003_setVolume(rcv.param);
				}
				break;
            case VS_MSG_LOOP:
                if (rcv.param) { VS1003_setLoop(true); }
                else { VS1003_setLoop(false); }
                break;
			default:
				break;
		}
	}    
}

void VS1003_setVolume(uint8_t vol) {
  if ((vol < 1) || (vol > 100)) return;
  int x = log10f(vol)*1000;
  uint8_t new_reg_value = map(x, 0, 2000, 0xFE, 0x00);//vol;
  last_volume = vol;
  uint16_t value = new_reg_value;
  value <<= 8;
  value |= new_reg_value;

  VS1003_write_register(SCI_VOL,value); // VOL
  
  #ifdef USE_LCD_UI
  ui_update_volume();
  #endif
}

uint8_t VS1003_getVolume(void) {
    return last_volume;
}

/****************************************************************************/

void VS1003_playChunk(const uint8_t* data, size_t len) {
  VS1003_sdi_send_buffer(data,len);
}

void VS1003_play_next(void) {
    switch (StreamState) {
        case STREAM_FILE_FILL_BUFFER:
        case STREAM_FILE_GET_DATA:
            if (dir_flag) {
                VS1003_play_next_audio_file_from_directory();
            }
            break;
        case STREAM_HOME:
        case STREAM_HTTP_FILL_BUFFER:
        case STREAM_HTTP_GET_DATA:
            VS1003_stop();
            VS1003_play_next_http_stream_from_list();
            break;
        default:
            break;
    }
}

void VS1003_play_prev(void) {
    switch (StreamState) {
        case STREAM_FILE_FILL_BUFFER:
        case STREAM_FILE_GET_DATA:
            if (dir_flag) {
                //TODO: This need to be implemented
                //VS1003_play_prev_audio_file_from_directory();
            }
            break;
        case STREAM_HOME:
        case STREAM_HTTP_FILL_BUFFER:
        case STREAM_HTTP_GET_DATA:
            VS1003_stop();
            VS1003_play_prev_http_stream_from_list();
            break;
        default:
            break;
    }
}
  
static void VS1003_startPlaying(void) {
    VS1003_sdi_send_zeroes(10);
}
  
static void VS1003_stopPlaying(void) {
    VS1003_sdi_send_zeroes(2048);
    ringbuffer_clear();
}
 
uint8_t is_audio_file (char* name) {
    if (strstr(name, ".MP3") || strstr(name, ".WMA") || strstr(name, ".MID") || strstr(name, ".mp3") || strstr(name, ".wma") || strstr(name, ".mid")) {
        return 1;
    }
    return 0;
 }

/* This is needed for directory playing - it stops playing
 and closes current file, but doesn't close directory and 
 leaves flag unchanged */

static void VS1003_soft_stop (void) {
    //Can be used only if it is actually playing from file
    if (StreamState == STREAM_FILE_GET_DATA || StreamState == STREAM_FILE_PLAY_REST || StreamState == STREAM_FILE_FILL_BUFFER) { 
        SYS_FS_FileClose(fsrc);
        VS1003_stopPlaying();
        StreamState = STREAM_HOME;        
    }
}

static void VS1003_handle_end_of_file (void) {
    int32_t res;
    
    if (dir_flag) {
        VS1003_play_next_audio_file_from_directory();   //it handles loops
    }
    else {
        if (loop_flag) {
            SYS_CONSOLE_PRINT("Loop flag set - rewinding\r\n");
            res = SYS_FS_FileSeek(fsrc, 0, SYS_FS_SEEK_SET);
            if (res == -1) SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "f_lseek ERROR\r\n");
            StreamState = STREAM_FILE_FILL_BUFFER;
        }
        else {
            SYS_CONSOLE_PRINT("Loop flag cleared - closing file\r\n");
            SYS_FS_FileClose(fsrc);
            VS1003_stopPlaying();
            mediainfo_clean();
            #ifdef USE_LCD_UI
            ui_clear_content_info();
            #endif
            StreamState = STREAM_HOME;
        }
    }    
}
  
  
void VS1003_play_next_audio_file_from_directory (void) {
    SYS_FS_FSTAT info;
    char buf[257];
    char lfn_buf[300];
    
    if(!dir_flag) return;       //currently we are not playing directory
    
    info.lfname = lfn_buf;
    info.lfsize = sizeof(lfn_buf);
    
    uint32_t timeout = millis();
    
    while (SYS_FS_DirRead(vsdir, &info) == SYS_FS_RES_SUCCESS) {
        if (!info.fname[0]) {           //Empty string - end of directory
            if (loop_flag) {
                SYS_CONSOLE_PRINT("Loop flag set - rewinding dir\r\n");
                SYS_FS_DirRewind(vsdir);
            }
            else {
                SYS_CONSOLE_PRINT("Loop flag cleared - stop playback\r\n");
                VS1003_stop();          //It handles closing dir and resetting dir_flag
                return;
            }
        }
        else {
            if (is_audio_file(info.fname)) {
                int ret = snprintf(buf, sizeof(buf)-1, "%s/%s", uri.server, info.fname);
                if (ret <= sizeof(buf)-1) {
                    VS1003_soft_stop();
                    VS1003_play_file(buf);
                }
                return;
            }
        }
        
        if ((uint32_t)(millis()-timeout) > 500) {
            //This is needed in case of empty directory
            break;
        }
    }
    
    return;
}

/*Always call VS1003_stop() before calling that function*/
void VS1003_play_http_stream(const char* url) {
    if ((StreamState == STREAM_HOME) || (StreamState == STREAM_HTTP_RECONNECT_WAIT)) {
        if (parse_url(url, strlen(url), &uri)) {
            StreamState = STREAM_HTTP_BEGIN;
            ReconnectStrategy = RECONNECT_WAIT_SHORT;
            ReconnectLimit = RECONNECT_LIMIT;
        }
        else {
            StreamState = STREAM_HOME;
            ReconnectStrategy = DO_NOT_RECONNECT;
        }
        mediainfo_type_set(MEDIA_TYPE_STREAM);
        VS1003_startPlaying();
    }
}

bool VS1003_play_http_stream_by_id(uint16_t id) {
    char name[32];
    
    memset(name, 0x00, sizeof(name));
	char* url = get_station_url_from_file(id, name, sizeof(name)-1);
	if (url) {
		VS1003_stop();
        mediainfo_title_set(name);
        #ifdef USE_LCD_UI
        ui_update_content_info(mediainfo_title_get());
        ui_set_selected_stream_id(id);
        #endif
		VS1003_play_http_stream(url);
        current_stream_ind = id;
        return true;
	}
    return false;
}

void VS1003_play_next_http_stream_from_list(void) {
    char name[32];
    
    memset(name, 0x00, sizeof(name));
    current_stream_ind++;
    char* url = get_station_url_from_file(current_stream_ind, name, sizeof(name)-1);
    if (url == NULL) {
        //Function returned NULL, there is no stream with such ind
        //Try again from the beginning
        current_stream_ind = 1;
        url = get_station_url_from_file(current_stream_ind, name, sizeof(name)-1);
        if (url == NULL) return;
    }
    VS1003_stop();
    mediainfo_title_set(name);
    #ifdef USE_LCD_UI
    ui_update_content_info(mediainfo_title_get());
    ui_set_selected_stream_id(current_stream_ind);
    #endif
    VS1003_play_http_stream(url);
}

void VS1003_play_prev_http_stream_from_list(void) {
    char name[32];
    
    memset(name, 0x00, sizeof(name));
    current_stream_ind--;
    if (current_stream_ind < 1) { current_stream_ind = get_max_stream_id(); }
    char* url = get_station_url_from_file(current_stream_ind, name, sizeof(name)-1);
    if (url == NULL) return;
    VS1003_stop();
    mediainfo_title_set(name);
    #ifdef USE_LCD_UI
    ui_update_content_info(mediainfo_title_get());
    ui_set_selected_stream_id(current_stream_ind);
    #endif
    VS1003_play_http_stream(url);
}

/*Always call VS1003_stop() or VS1003_soft_stop() before calling that function*/
void VS1003_play_file (char* url) {
    if (StreamState != STREAM_HOME) return;
    
    fsrc = SYS_FS_FileOpen(url, SYS_FS_FILE_OPEN_READ);
    if (fsrc == SYS_FS_HANDLE_INVALID) {
        SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "SYS_FS_FileOpen error %d\r\n", SYS_FS_Error());
        StreamState = STREAM_HOME;
        return;
    }
    
    //Read ID3
    {
        char id3[128];
        if (SYS_FS_FileSeek(fsrc, -128, SYS_FS_SEEK_END) == -1) {
            SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "ID3 SYS_FS_FileSeek (end-128) error\r\n");
            SYS_FS_FileClose(fsrc);
            return;
        }
        if (SYS_FS_FileRead(fsrc, id3, 128) == -1) {
            SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "ID3 SYS_FS_FileRead error\r\n");
            SYS_FS_FileClose(fsrc);
            return;
        }
        //Move to beginning
        if (SYS_FS_FileSeek(fsrc, 0, SYS_FS_SEEK_SET) == -1) {
            SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "ID3 SYS_FS_FileSeek (begin) error\r\n");
            SYS_FS_FileClose(fsrc);
            return;
        }
        if (strncmp(id3, "TAG", 3)) {
            SYS_CONSOLE_PRINT("File doesn't have an ID3 tag\n\n");
            mediainfo_title_set("No ID3");
        }
        else {
            unsigned char *s = (unsigned char*)id3+3;
            
            /* Id3 tag elements don't necessarily end in a '\0' if they
            occupy the full 30 character space. Thus, we'll have to
            nul-terminate all strings just to be sure. */
            mediainfo_title_set((const char*)s);
            s += 30;
            mediainfo_artist_set((const char*)s);
            s += 30;
            mediainfo_album_set((const char*)s);
            s += 30;
            mediainfo_year_set((const char*)s);
            s += 4;
            mediainfo_comment_set((const char*)s);
            s += 30;

            /* The genre field is not encoded as a string, but just one byte
            that indexes the decoding table. If memory is too expensive,
            the whole field may be ignored or just shown as a number. */
            mediainfo_genre_set(*s);

            /* And now the final printing */
            SYS_CONSOLE_PRINT("Title  : %-30s  Artist: %s\r\n", mediainfo_title_get(), mediainfo_artist_get());
            SYS_CONSOLE_PRINT("Album  : %-30s  Year  : %4s\r\n", mediainfo_album_get(), mediainfo_year_get());
            SYS_CONSOLE_PRINT("Comment: %-30s  Genre : %s\r\n", mediainfo_comment_get(), mediainfo_genre_get());
        }
    }
    
    #ifdef USE_LCD_UI
    ui_update_content_info(mediainfo_title_get());
    #endif
    mediainfo_type_set(MEDIA_TYPE_FILE);
    StreamState = STREAM_FILE_FILL_BUFFER;
    VS1003_startPlaying();         //Start playing song
}


void VS1003_play_dir (const char* url) {
    vsdir = SYS_FS_DirOpen(url);
    if (vsdir == SYS_FS_HANDLE_INVALID) {
        SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "SYS_FS_DirOpen error\r\n");
        return;
    }
    dir_flag = true;
    strncpy(uri.server, url, sizeof(uri.server)-1);		//we use uri.server to store current directory path
    VS1003_play_next_audio_file_from_directory();
}

void VS1003_stop(void) {
    //Can be stopped only if it is actually playing
    switch (StreamState) {
        case STREAM_HTTP_BEGIN:
        case STREAM_HTTP_SOCKET_OBTAINED:
        case STREAM_HTTP_SEND_REQUEST:
        case STREAM_HTTP_PROCESS_HEADER:
        case STREAM_HTTP_FILL_BUFFER:
        case STREAM_HTTP_GET_DATA:
            if(VS_Socket != INVALID_SOCKET) {
                if (!TCPIP_TCP_Disconnect(VS_Socket)) {
                    TCPIP_TCP_Abort(VS_Socket, true);
                }
                VS_Socket = INVALID_SOCKET;       
            }
            break;
        case STREAM_FILE_FILL_BUFFER:    
        case STREAM_FILE_GET_DATA:
        case STREAM_FILE_PLAY_REST:
            SYS_FS_FileClose(fsrc);
            if (dir_flag) {
                SYS_FS_DirClose(vsdir);
                dir_flag = false;
            }
            break;
        default:
            return;
            break;
    }
    VS1003_stopPlaying();
    mediainfo_clean();
    #ifdef USE_LCD_UI
    ui_clear_content_info();
    #endif
    StreamState = STREAM_HOME;
}

void VS1003_setLoop(bool val) {
    loop_flag = val;
}

bool VS1003_getLoop(void) {
    return loop_flag;
}

StreamState_t VS1003_getStreamState(void) {
    return StreamState;
}

void VS1003_send_cmd_thread_safe(uint8_t cmd, uint32_t param) {
	vs1003cmd_t command;
	command.cmd = cmd;
	command.param = param;
	if (xQueueSend(vsQueueHandle, (void*)&command, portMAX_DELAY)) {
		SYS_CONSOLE_PRINT("Sending thread safe command to VS1003: %d\r\n", command.cmd);
	}
}


//#define BASE 0x1800
//
//void VS1003_read_spectrum_analyzer(void) {
//    //SYS_CONSOLE_PRINT("Reading spectrum analyzer data: ");
//    VS1003_write_register(SCI_WRAMADDR, BASE+2);
//    int bands = VS1003_read_register(SCI_WRAM);
//    SYS_CONSOLE_PRINT("Bands: %d\r\n", bands);
//    VS1003_write_register(SCI_WRAMADDR, BASE+4);
//    for (int i=0; i<bands; i++) {
//        VS1003_read_register(SCI_WRAM);
//        //SYS_CONSOLE_PRINT("%d, ", val);
//    }
//    //SYS_CONSOLE_PRINT("\r\n");
//}
//
//
//void VS1003_set_bands(void) {
//    int bands = 14;
//    int i;
//    
//    static const short frequency[] = {50, 79, 126, 200, 317, 504, 800, 1270, 2016, 3200, 5080, 8063, 12800, 20319};
//    /* send new frequencies */
//    VS1003_write_register(SCI_WRAMADDR, BASE+0x68);
//    VS1003_write_register(SCI_WRAMADDR, BASE+0x68);
//    for (i=0;i<bands;i++) {
//        VS1003_write_register(SCI_WRAM, frequency[i]);
//    }
//    if (i < 23) {
//        VS1003_write_register(SCI_WRAM, 25000);
//    }
//    /* activate */
//    VS1003_write_register(SCI_WRAMADDR, BASE+1);
//    VS1003_write_register(SCI_WRAM, 0);
//}