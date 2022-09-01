#include <stdio.h>
#include <ctype.h>
#include <stdio.h>

#include "common.h"

FRESULT FormatSpiFlashDisk (void) {
    FRESULT res;
    char buffer[4096];
    res = f_mkfs("0:", FM_ANY, 0, buffer, sizeof(buffer));
    if (res != FR_OK) {printf("f_mmkfs error code: %i\r\n", res);}
    else {printf("f_mkfs OK\r\n");}
    return res;
}


unsigned char BcdToByte(unsigned char bcd) {
    return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

unsigned char ByteToBcd(unsigned char i) {
    return ((i / 10) << 4) + (i % 10);
}

unsigned char StringToMACAddress(BYTE* str, MAC_ADDR* MACAddress) {
    int values[6];
    unsigned char i;
    for (i=0; i<17; i++) {
        if (i % 3 != 2 && !isxdigit(str[i])) {
            return 0;
        }
        if (i % 3 == 2 && str[i] != ':') {
            return 0;
        }
    }
    if (str[17] != '\0') return 0;
    
    if (sscanf(str, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]) == 6) {
        for (i=0; i<6; i++) {
            MACAddress->v[i] = (uint8_t)values[i];
        }
        return 1;
    }
    return 0;
}


long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


http_res_t parse_http_headers(char* str, size_t len, uri_t* uri) {
	char* tok;
	
	if (str == NULL) { return HTTP_HEADER_ERROR; }
	
	if (strncmp(str, "HTTP/", 5) != 0) {
		return HTTP_HEADER_ERROR;
	}
	tok = strchr(str, ' ');
	if (tok) {
		char* code = tok+1;
		if (code >= str+len) { return HTTP_HEADER_ERROR; }
		if (strncmp(code, "200 OK", 6) == 0) {
			return HTTP_HEADER_OK;
		}
		else if ( (strncmp(code, "301 Moved Permanently", 21) == 0) || (strncmp(code, "302 Moved Temporarily", 21) == 0) ) {
			char* location = strstr(code, "Location: ");
			if (location) {
				char* newurl = location+10;
				if (newurl >= str+len) { return HTTP_HEADER_ERROR; }
				tok = strstr(newurl, "\r\n");
				if(!tok || (tok >= str+len)) return HTTP_HEADER_ERROR;
				*tok = '\0';
                if (parse_url(newurl, strlen(newurl), uri)) {
                    return HTTP_HEADER_REDIRECTED;
                }
				return HTTP_HEADER_ERROR;
			}
		}
		else {
			printf("Unsupported code\r\n");
			return HTTP_HEADER_ERROR;
		}
	}
}


uint8_t parse_url (char* str, size_t len, uri_t* uri) {
	char* url = str;
	char* tok;
	char* rest;
	char* port;
	
	if (uri == NULL) return 0;
	
	tok = strstr(url, "://");
	if (tok) {
		if (tok >= str+len) return 0;
		*tok = '\0';
		if (strncasecmp(url, "https", 5) == 0) {
			//printf("It is SSL\r\n");
			uri->https = 1;
			uri->port = 443;
		}
		else if (strncasecmp(url, "http", 4) == 0) {
			//printf("It is normal HTTP\r\n");
			uri->https = 0;
			uri->port = 80;
		}
		else {
			//printf("It is different protocol\r\n");
			return 0;
		}
		rest = tok+3;
		if (rest >= str+len) return 0;
		url = rest;
	}
	else {
		//printf("No protocol specified - assuming HTTP\r\n");
		uri->https = 0;
		uri->port = 80;
	}
	
	//printf("%s\r\n", url);
	
	tok = strchr(url, ':');
	if (tok) {
		if (tok >= str+len) return 0;
		*tok = '\0';
		rest = tok+1;
		if (rest >= str+len) return 0;
		//printf("Wykryto dwukropek: %s\r\n", rest);
		tok = strchr(rest, '/');
		if (tok) {
			if (tok >= str+len) return 0;
			//printf("Wykryto znak /, wszystko ok\r\n");
			//*tok = '\0';
			port = rest;
			uri->port = atoi(port);
			if (uri->port == 0) {
				//printf("Nieprawid?owy port %d\r\n", uri->port);
				return 0;
			}
			rest = tok;
		}
		else {
			//printf("Oczekiwano znaku /, cos jest nie tak\r\n");
			return 0;
		}
	}
	else {
		//printf("Nie wykryto dwukropka\r\n");
		tok = strchr(url, '/');
		if (tok) {
			if (tok >= str+len) return 0;
			*tok = '\0';
			rest = tok+1;
			if (rest >= str+len) return 0;
		}
		else {
			//printf("Oczekiwano znaku /, cos jest nie tak\r\n");
			return 0;
		}
	}
	//printf("server: %s\r\n", url);
	strncpy(uri->server, url, URI_SERVER_SIZE);
	//printf("reszta: %s\r\n", rest);
	strncpy((uri->file)+1, rest, URI_FILE_SIZE-1);
	uri->file[0] = '/';
	return 1;
}