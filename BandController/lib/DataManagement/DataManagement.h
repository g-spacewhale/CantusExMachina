/* ------------------------------------------------- */
//
//      BandController - Class to manage the SD Data
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#ifndef DataManagement_h
#define DataManagement_h

#include <SD.h>
#include <Song.h>
#include <MidiFileHandler.h>

#define _RETURNVAL_MIDI_DIR_NOT_FOUND -1

#define _MAX_SONG_COUNT 10


class DataManagement
{
  public:
    DataManagement(int pinCS);
    char begin();
    //char getLanguageFile(char language, File &languageFile);
    char loadSongs();
    char parseSongInfos();
    Song getSong(int pos);
    uint16_t getSongCount();

  private:
    int _pinCS;
    Song *_songs[_MAX_SONG_COUNT];
    uint16_t _songCount;
};

#endif
