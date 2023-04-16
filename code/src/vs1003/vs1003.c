/*
 Copyright (C) 2012 Andy Karpov <andy.karpov@gmail.com>
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
 
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "peripheral/coretimer/plib_coretimer.h"
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
#include "mediainfo.h"
#include "ringbuffer.h"
#include "../common.h"

#define VS_DREQ_TRIS    TRISCbits.TRISC1
#define VS_DREQ_PIN     PORTCbits.RC1       //It is input!
#define VS_CS_TRIS      TRISEbits.TRISE7
#define VS_CS_PIN		LATEbits.LATE7
#define VS_DCS_TRIS     TRISEbits.TRISE6
#define VS_DCS_PIN      LATEbits.LATE6
#define VS_RESET_TRIS   TRISCbits.TRISC2
#define VS_RESET_PIN	LATCbits.LATC2

#define vs1003_chunk_size 32

/****************************************************************************/

// VS1003 SCI Write Command byte is 0x02
#define VS_WRITE_COMMAND 0x02

// VS1003 SCI Read COmmand byte is 0x03
#define VS_READ_COMMAND  0x03

// SCI Registers

#define SCI_MODE            0x0
#define SCI_STATUS          0x1
#define SCI_BASS            0x2
#define SCI_CLOCKF          0x3
#define SCI_DECODE_TIME     0x4
#define SCI_AUDATA          0x5
#define SCI_WRAM            0x6
#define SCI_WRAMADDR        0x7
#define SCI_HDAT0           0x8
#define SCI_HDAT1           0x9
#define SCI_AIADDR          0xa
#define SCI_VOL             0xb
#define SCI_AICTRL0         0xc
#define SCI_AICTRL1         0xd
#define SCI_AICTRL2         0xe
#define SCI_AICTRL3         0xf
#define SCI_num_registers   0xf

// SCI_MODE bits

#define SM_DIFF             0
#define SM_LAYER12          1
#define SM_RESET            2
#define SM_OUTOFWAV         3
#define SM_EARSPEAKER_LO    4
#define SM_TESTS            5
#define SM_STREAM           6
#define SM_EARSPEAKER_HI    7
#define SM_DACT             8
#define SM_SDIORD           9
#define SM_SDISHARE         10
#define SM_SDINEW           11
#define SM_ADPCM            12
#define SM_ADCPM_HP         13
#define SM_LINE_IN          14

#define GENRES 148

char *genres[GENRES] = {
  "Blues", "Classic Rock", "Country", "Dance",
  "Disco", "Funk", "Grunge", "Hip-Hop",
  "Jazz", "Metal", "New Age", "Oldies",
  "Other", "Pop", "R&B", "Rap",
  "Reggae", "Rock", "Techno", "Industrial",
  "Alternative", "Ska", "Death Metal", "Pranks",
  "Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop",
  "Vocal", "Jazz+Funk", "Fusion", "Trance",
  "Classical", "Instrumental", "Acid", "House",
  "Game", "Sound Clip", "Gospel", "Noise",
  "AlternRock", "Bass", "Soul", "Punk",
  "Space", "Meditative", "Instrumental Pop", "Instrumental Rock",
  "Ethnic", "Gothic", "Darkwave", "Techno-Industrial",
  "Electronic", "Pop-Folk", "Eurodance", "Dream",
  "Southern Rock", "Comedy", "Cult", "Gangsta",
  "Top 40", "Christian Rap", "Pop/Funk", "Jungle",
  "Native American", "Cabaret", "New Wave", "Psychadelic",
  "Rave", "Showtunes", "Trailer", "Lo-Fi",
  "Tribal", "Acid Punk", "Acid Jazz", "Polka",
  "Retro", "Musical", "Rock & Roll", "Hard Rock",
  "Folk", "Folk/Rock", "National folk", "Swing",
  "Fast-fusion", "Bebob", "Latin", "Revival",
  "Celtic", "Bluegrass", "Avantgarde", "Gothic Rock",
  "Progressive Rock", "Psychedelic Rock", "Symphonic Rock", "Slow Rock",
  "Big Band", "Chorus", "Easy Listening", "Acoustic",
  "Humour", "Speech", "Chanson", "Opera",
  "Chamber Music", "Sonata", "Symphony", "Booty Bass",
  "Primus", "Porn Groove", "Satire", "Slow Jam",
  "Club", "Tango", "Samba", "Folklore",
  "Ballad", "Powder Ballad", "Rhythmic Soul", "Freestyle",
  "Duet", "Punk Rock", "Drum Solo", "A Capella",
  "Euro-House", "Dance Hall", "Goa", "Drum & Bass",
  "Club House", "Hardcore", "Terror", "Indie",
  "BritPop", "NegerPunk", "Polsk Punk", "Beat",
  "Christian Gangsta", "Heavy Metal", "Black Metal", "Crossover",
  "Contemporary C", "Christian Rock", "Merengue", "Salsa",
  "Thrash Metal", "Anime", "JPop", "SynthPop"
};

QueueHandle_t vsQueueHandle;

static SYS_FS_HANDLE fsrc;
static SYS_FS_HANDLE vsdir;

static TCP_SOCKET	VS_Socket = INVALID_SOCKET;
static uri_t uri;
static bool loop_flag = false;
static bool dir_flag = false;
static uint8_t last_volume = 0;

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

static StreamState_t StreamState = STREAM_HOME;

typedef enum {
    DO_NOT_RECONNECT = 0,
    RECONNECT_IMMEDIATELY,
    RECONNECT_WAIT_LONG,
    RECONNECT_WAIT_SHORT
} ReconnectStrategy_t;

static ReconnectStrategy_t ReconnectStrategy = DO_NOT_RECONNECT;

typedef enum {
    FEED_RET_NO_DATA_NEEDED = 0,
    FEED_RET_OK,
    FEED_RET_BUFFER_EMPTY
} feed_ret_t;

// Register names

typedef enum {
    reg_name_MODE = 0,
    reg_name_STATUS,
    reg_name_BASS,
    reg_name_CLOCKF,
    reg_name_DECODE_TIME,
    reg_name_AUDATA,
    reg_name_WRAM,
    reg_name_WRAMADDR,
    reg_name_HDAT0,
    reg_name_HDAT1,
    reg_name_AIADDR,
    reg_name_VOL,
    reg_name_AICTRL0,
    reg_name_AICTRL1,
    reg_name_AICTRL2,
    reg_name_AICTRL3
} register_names_t;

const char * register_names[] =
{
  "MODE",
  "STATUS",
  "BASS",
  "CLOCKF",
  "DECODE_TIME",
  "AUDATA",
  "WRAM",
  "WRAMADDR",
  "HDAT0",
  "HDAT1",
  "AIADDR",
  "VOL",
  "AICTRL0",
  "AICTRL1",
  "AICTRL2",
  "AICTRL3",
};

static void VS1003_startPlaying(void);
static void VS1003_stopPlaying(void);
static inline void await_data_request(void);
static inline void control_mode_on(void);
static inline void control_mode_off(void);
static inline void data_mode_on(void);
static inline void data_mode_off(void);
static uint8_t VS1003_SPI_transfer(uint8_t outB);
static void VS1003_soft_stop (void);
static void VS1003_handle_end_of_file (void);
static feed_ret_t VS1003_feed_from_buffer (void);
//static void VS1003_set_bands(void);


/****************************************************************************/

uint16_t VS1003_read_register(uint8_t _reg) {
  uint16_t result;
  control_mode_on();
  CORETIMER_DelayUs(1); // tXCSS
  VS1003_SPI_transfer(VS_READ_COMMAND); // Read operation
  VS1003_SPI_transfer(_reg); // Which register
  result = VS1003_SPI_transfer(0xff) << 8; // read high byte
  result |= VS1003_SPI_transfer(0xff); // read low byte
  CORETIMER_DelayUs(1); // tXCSH
  await_data_request();
  control_mode_off();
  return result;
}

/****************************************************************************/

void VS1003_write_register(uint8_t _reg,uint16_t _value) {
  control_mode_on();
  CORETIMER_DelayUs(1); //delay_us(1); // tXCSS
  VS1003_SPI_transfer(VS_WRITE_COMMAND); // Write operation
  VS1003_SPI_transfer(_reg); // Which register
  VS1003_SPI_transfer(_value >> 8); // Send hi byte
  VS1003_SPI_transfer(_value & 0xff); // Send lo byte
  CORETIMER_DelayUs(1); //delay_us(1); // tXCSH
  await_data_request();
  control_mode_off();
}

/****************************************************************************/

void VS1003_sdi_send_buffer(const uint8_t* data, int len) {
  int chunk_length;
  
  data_mode_on();
  while ( len ) {
    await_data_request();
    CORETIMER_DelayUs(3); //delay_us(3);
    chunk_length = min(len, vs1003_chunk_size);
    len -= chunk_length;
    while ( chunk_length-- ) VS1003_SPI_transfer(*data++);
  }
  data_mode_off();
}

/****************************************************************************/

void VS1003_sdi_send_chunk(const uint8_t* data, int len) {
    if (len > 32) return;
    data_mode_on();
    await_data_request();
    while ( len-- ) VS1003_SPI_transfer(*data++);
    data_mode_off();
}

/****************************************************************************/

void VS1003_sdi_send_zeroes(int len) {
  int chunk_length;  
  data_mode_on();
  while ( len ) {
    await_data_request();
    chunk_length = min(len,vs1003_chunk_size);
    len -= chunk_length;
    while ( chunk_length-- ) VS1003_SPI_transfer(0);
  }
  data_mode_off();
}

/****************************************************************************/

static feed_ret_t VS1003_feed_from_buffer (void) {
    uint8_t data[32];

    if (!VS_DREQ_PIN) {
        return FEED_RET_NO_DATA_NEEDED;
    }
    
    do {
        if (get_num_of_bytes_in_ringbuffer() < 32) return FEED_RET_BUFFER_EMPTY;
        
        uint16_t w = read_array_from_ringbuffer(data, 32);
        if (w == 32) VS1003_sdi_send_chunk(data, 32);
        asm("nop");
        asm("nop");
        asm("nop");
    } while(VS_DREQ_PIN);

    return FEED_RET_OK;
}


void VS1003_handle(void) {   
	static uint32_t	timer;
    static IP_MULTI_ADDRESS ServerAddress;
    TCPIP_DNS_RESULT dnsres;
    static uri_t tempUri;
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
                ReconnectStrategy = RECONNECT_WAIT_LONG;
                SYS_CONSOLE_PRINT("Internet radio: socket disconnected - reseting\r\n");
				break;
			}
            
            w = TCPIP_TCP_ArrayGet(VS_Socket, data, 32);
            if (w) {
                http_res_t http_result = parse_http_headers((char*)data, w, &tempUri);
                switch (http_result) {
                    case HTTP_HEADER_ERROR:
                        SYS_CONSOLE_PRINT("Parsing headers error\r\n");
                        prepare_http_parser();
                        ReconnectStrategy = RECONNECT_WAIT_LONG;
                        StreamState = STREAM_HTTP_CLOSE;
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
                        memcpy(&uri, &tempUri, sizeof(uri_t));
                        ReconnectStrategy = RECONNECT_IMMEDIATELY;
                        StreamState = STREAM_HTTP_CLOSE;
                        break;
                    case HTTP_HEADER_IN_PROGRESS:
                        break;
                    default:
                        break;
                }
            }
            
            if ( (uint32_t)(millis()-timer) > 2000) {
                //There was no data in 2 seconds - reconnect
                SYS_CONSOLE_PRINT("Internet radio: no header timeout - reseting\r\n");
                ReconnectStrategy = RECONNECT_WAIT_LONG;
                StreamState = STREAM_HTTP_CLOSE;
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
                break;
            }
    
			if(TCPIP_TCP_WasReset(VS_Socket))
			{
				StreamState = STREAM_HTTP_CLOSE;
                ReconnectStrategy = RECONNECT_WAIT_LONG;
                SYS_CONSOLE_PRINT("Internet radio: socket disconnected - reseting\r\n");
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}
    
            if ( (uint32_t)(millis()-timer) > 5000) {
                //There was no data in 5 seconds - reconnect
                SYS_CONSOLE_PRINT("Internet radio: no new data timeout - reseting\r\n");
                ReconnectStrategy = RECONNECT_WAIT_LONG;
                StreamState = STREAM_HTTP_CLOSE;
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
                SYS_CONSOLE_PRINT("Internet radio: socket disconnected - reseting\r\n");
				// Do not break;  We might still have data in the TCP RX FIFO waiting for us
			}
            if ( (uint32_t)(millis()-timer) > 5000) {
                //There was no data in 5 seconds - reconnect
                SYS_CONSOLE_PRINT("Internet radio: no new data timeout - reseting\r\n");
                ReconnectStrategy = RECONNECT_WAIT_LONG;
                StreamState = STREAM_HTTP_CLOSE;
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
                    break;
                case RECONNECT_IMMEDIATELY:
                    StreamState = STREAM_HTTP_BEGIN;
                    break;
                case RECONNECT_WAIT_LONG:
                case RECONNECT_WAIT_SHORT:
                    StreamState = STREAM_HTTP_RECONNECT_WAIT;
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


/****************************************************************************/

void VS1003_begin(void) {
  // Keep the chip in reset until we are ready
  VS_RESET_TRIS = 0;
  VS_RESET_PIN = 0;

  // The SCI and SDI will start deselected
  VS_CS_TRIS = 0;
  VS_CS_PIN = 1;
  VS_DCS_TRIS = 0;
  VS_DCS_PIN = 1;

  // DREQ is an input
  VS_DREQ_TRIS = 1;

  // Boot VS1003
  SYS_CONSOLE_PRINT("Booting VS1003...\r\n");

  vTaskDelay(1);

  // init SPI slow mode
  //SPI4 configuration     
  SPI1CON = (_SPI1CON_ON_MASK  | _SPI1CON_CKE_MASK | _SPI1CON_MSTEN_MASK);    //8 bit master mode, CKE=1, CKP=0
  SPI1BRG = 79; //(GetPeripheralClock()-1ul)/2ul/250000;       //250 kHz  

  // release from reset
  VS_RESET_PIN = 1;
  
  // Declick: Immediately switch analog off
  VS1003_write_register(SCI_VOL,0xffff); // VOL

  /* Declick: Slow sample rate for slow analog part startup */
  VS1003_write_register(SCI_AUDATA,10);

  vTaskDelay(100);

  /* Switch on the analog parts */
  VS1003_write_register(SCI_VOL,0xfefe); // VOL
  
  SYS_CONSOLE_PRINT("VS1003 still booting\r\n");

  VS1003_write_register(SCI_AUDATA,44101); // 44.1kHz stereo

  VS1003_write_register(SCI_VOL,0x2020); // VOL
  
  // soft reset
  VS1003_write_register(SCI_MODE, (1 << SM_SDINEW) | (1 << SM_RESET) );
  vTaskDelay(1);
  await_data_request();
  VS1003_write_register(SCI_CLOCKF,0xF800); // Experimenting with highest clock settings
  vTaskDelay(1);
  await_data_request();

  // Now you can set high speed SPI clock   
  SPI1CON = (_SPI1CON_ON_MASK  | _SPI1CON_CKE_MASK | _SPI1CON_MSTEN_MASK);    //8 bit master mode, CKE=1, CKP=0
  SPI1BRG = 2; //8MHz

  SYS_CONSOLE_PRINT("VS1003 Set\r\n");
  VS1003_printDetails();
  SYS_CONSOLE_PRINT("VS1003 OK\r\n");
  
  vsQueueHandle = xQueueCreate(16, sizeof(vs1003cmd_t));
  if (vsQueueHandle == NULL) {
      SYS_CONSOLE_PRINT("Can't create VS queue\r\n");
  }
  else {
      SYS_CONSOLE_PRINT("VS queue created\r\n");
  }
}

/****************************************************************************/

void VS1003_setVolume(uint8_t vol) {
  if ((vol < 1) || (vol > 100)) return;
  int x = log10f(vol)*1000;
  uint8_t new_reg_value = map(x, 0, 2000, 0xFE, 0x00);//vol;
  last_volume = vol;
  uint16_t value = new_reg_value;
  value <<= 8;
  value |= new_reg_value;

  VS1003_write_register(SCI_VOL,value); // VOL
}

uint8_t VS1003_getVolume(void) {
    return last_volume;
}

/****************************************************************************/

void VS1003_playChunk(const uint8_t* data, size_t len) {
  VS1003_sdi_send_buffer(data,len);
}

/****************************************************************************/

void VS1003_print_byte_register(uint8_t reg) {
  char extra_tab = strlen(register_names[reg]) < 5 ? '\t' : 0;
  SYS_CONSOLE_PRINT("%02x %s\t%c = 0x%02x\r\n", reg, register_names[reg], extra_tab, VS1003_read_register(reg));
}

/****************************************************************************/

void VS1003_printDetails(void) {
  SYS_CONSOLE_PRINT("VS1003 Configuration:\r\n");
  int i = 0;
  while ( i <= SCI_num_registers )
    VS1003_print_byte_register(i++);
}

/****************************************************************************/

//void VS1003_loadUserCode(const uint16_t* buf, size_t len) {
//  while (len) {
//    uint16_t addr = *buf++; len--;
//    uint16_t n = *buf++; len--;
//    if (n & 0x8000U) { /* RLE run, replicate n samples */
//      n &= 0x7FFF;
//      uint16_t val = *buf++; len--;
//      while (n--) {
//	    SYS_CONSOLE_PRINT("W %02x: %04x\r\n", addr, val);
//        VS1003_write_register(addr, val);
//      }
//    } else {           /* Copy run, copy n samples */
//      while (n--) {
//        uint16_t val = *buf++; len--;
//        SYS_CONSOLE_PRINT("W %02x: %04x\r\n", addr, val);
//        VS1003_write_register(addr, val);
//      }
//    }
//  }
//}

void VS1003_play_next(void) {
    switch (StreamState) {
        case STREAM_FILE_FILL_BUFFER:
        case STREAM_FILE_GET_DATA:
            if (dir_flag) {
                VS1003_play_next_audio_file_from_directory();
            }
            break;
        case STREAM_HTTP_FILL_BUFFER:
        case STREAM_HTTP_GET_DATA:
            VS1003_stop();
            VS1003_play_next_http_stream_from_list();
            break;
        default:
            break;
    }
}


static inline void await_data_request(void) {
    while ( !VS_DREQ_PIN );
}

static inline void control_mode_on(void) {
    VS_DCS_PIN = 1;
    VS_CS_PIN = 0;
}

static inline void control_mode_off(void) {
    VS_CS_PIN = 1;
}

static inline void data_mode_on(void) {
    VS_CS_PIN = 1;
    VS_DCS_PIN = 0;
}

static inline void data_mode_off(void) {
    VS_DCS_PIN = 1;
}
  
static void VS1003_startPlaying(void) {
  VS1003_sdi_send_zeroes(10);
}
  
static void VS1003_stopPlaying(void) {
    VS1003_sdi_send_zeroes(2048);
    ringbuffer_clear();
}
  
static uint8_t VS1003_SPI_transfer(uint8_t outB) {
    SPI1BUF = outB;
    while (SPI1STATbits.SPITBF);
    while (!SPI1STATbits.SPIRBF);
    return SPI1BUF;
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
                snprintf(buf, sizeof(buf)-1, "%s/%s", uri.server, info.fname);
                VS1003_soft_stop();
                VS1003_play_file(buf);
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
    if (StreamState != STREAM_HOME) return;
    
    if (parse_url(url, strlen(url), &uri)) {
        StreamState = STREAM_HTTP_BEGIN;
        ReconnectStrategy = RECONNECT_WAIT_SHORT;
    }
    else {
        StreamState = STREAM_HOME;
        ReconnectStrategy = DO_NOT_RECONNECT;
    }
    mediainfo_type_set(MEDIA_TYPE_STREAM);
    VS1003_startPlaying();
}

void VS1003_play_http_stream_by_id(uint16_t id) {
    char name[32];
    
    memset(name, 0x00, sizeof(name));
	char* url = get_station_url_from_file(id, name, sizeof(name)-1);
	if (url) {
		VS1003_stop();
        mediainfo_title_set(name);
		VS1003_play_http_stream(url);
	}
}

void VS1003_play_next_http_stream_from_list(void) {
    static int ind = 1;
    char name[32];
    
    memset(name, 0x00, sizeof(name));
    char* url = get_station_url_from_file(ind, name, sizeof(name)-1);
    if (url == NULL) {
        //Function returned NULL, there is no stream with such ind
        //Try again from the beginning
        ind = 1;
        url = get_station_url_from_file(ind, name, sizeof(name)-1);
        if (url == NULL) return;
    }
    else {
        ind++;
    }
    VS1003_stop();
    mediainfo_title_set(name);
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
        }
        else {
            unsigned char *s = (unsigned char*)id3+3;
            char title[31];
            char artist[31];
            char album[31];
            char year[5];
            char comment[31];
            char genre[31];
            
            /* Id3 tag elements don't necessarily end in a '\0' if they
            occupy the full 30 character space. Thus, we'll have to
            nul-terminate all strings just to be sure. */
            strncpy(title, (const char*)s, 30);
            title[30] = '\0';
            s += 30;

            strncpy(artist,(const char*) s, 30);
            artist[30] = '\0';
            s += 30;

            strncpy(album, (const char*)s, 30);
            album[30] = '\0';
            s += 30;

            strncpy(year, (const char*)s, 4);
            year[4] = '\0';
            s += 4;

            strncpy(comment, (const char*)s, 30);
            comment[30] = '\0';
            s += 30;

            /* The genre field is not encoded as a string, but just one byte
            that indexes the decoding table. If memory is too expensive,
            the whole field may be ignored or just shown as a number. */
            if (*s >= GENRES)
              strcpy(genre, "Unknown");
            else
              strcpy(genre, genres[*s]);

            /* And now the final printing */
            SYS_CONSOLE_PRINT("Title  : %-30s  Artist: %s\r\n", title, artist);
            mediainfo_title_set(title);
            mediainfo_artist_set(artist);
            SYS_CONSOLE_PRINT("Album  : %-30s  Year  : %4s\r\n", album, year);
            mediainfo_album_set(album);
            mediainfo_year_set(year);
            SYS_CONSOLE_PRINT("Comment: %-30s  Genre : %s\r\n", comment, genre);
            mediainfo_genre_set(genre);
        }
    }
    
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
    StreamState = STREAM_HOME;
}

void VS1003_setLoop(bool val) {
    loop_flag = val;
}

bool VS1003_getLoop(void) {
    return loop_flag;
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