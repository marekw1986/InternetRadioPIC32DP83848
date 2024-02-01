#ifndef _STREAM_LIST_H_
#define _STREAM_LIST_H_

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif
void initialize_stream_list(void);
uint16_t get_max_stream_id(void);
char* get_station_url_from_file(uint16_t number, char* stream_name, size_t stream_name_len);
char* get_station_url_from_file_use_seek(uint16_t number, char* stream_name, size_t stream_name_len, int32_t* cur_pos);
uint16_t parse_stream_data_line(char* line, size_t line_len, char* stream_name, size_t stream_name_len, char* stream_url, size_t stream_url_len);   
#ifdef	__cplusplus
}
#endif

#endif // _STREAM_LIST_H_