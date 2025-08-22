#ifndef DIR_PLAY_H
#define	DIR_PLAY_H

#include <stdint.h>
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

void VS1053_play_dir_alt (const char* path);
uint16_t count_audio_files_in_dir(const char* path);

#ifdef	__cplusplus
}
#endif

#endif	/* DIR_PLAY_H */