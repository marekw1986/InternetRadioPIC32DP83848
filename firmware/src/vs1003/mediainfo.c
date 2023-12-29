#include <stdio.h>
#include <string.h>
#include "mediainfo.h"

const char *genres[GENRES] = {
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

static char title[31];
static char artist[31];
static char album[31];
static char year[5];
static char comment[31];
static char genre[31];
static mediainfo_type_t type = MEDIA_TYPE_STREAM;

void mediainfo_clean (void) {
    title[0] = '\0';
    artist[0] = '\0';
    album[0] = '\0';
    year[0] = '\0';
    comment[0] = '\0';
    genre[0] = '\0';
}

char* mediainfo_title_get(void) {
    return title;
}

void mediainfo_title_set(const char* src) {
    strncpy(title, src, sizeof(title)-1);
}

char* mediainfo_artist_get(void) {
    return artist;
}

void mediainfo_artist_set(const char* src) {
    strncpy(artist, src, sizeof(artist)-1);
}

char* mediainfo_album_get(void) {
    return album;
}

void mediainfo_album_set(const char* src) {
    strncpy(album, src, sizeof(album)-1);
}

char* mediainfo_year_get(void) {
    return year;
}

void mediainfo_year_set(const char* src) {
    strncpy(year, src, sizeof(year)-1);
}

char* mediainfo_comment_get(void) {
    return comment;
}

void mediainfo_comment_set(const char* src) {
    strncpy(comment, src, sizeof(comment)-1);
}

char* mediainfo_genre_get(void) {
    return genre;
}

void mediainfo_genre_set(const char* src) {
    strncpy(genre, src, sizeof(genre)-1);
}

mediainfo_type_t mediainfo_type_get(void) {
    return type;
}

void mediainfo_type_set(mediainfo_type_t src) {
    type = src;
}