/* ------------------------------------------------- */
//
//      BandController - Class to manage the SD Data
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */


/* ------------------------------------------------- */
//      DATA STRUCTURE FOR SC CARD
//
//      |- grfx
//        |- btn_settings_rg.gif
//        |- ...
//      |- lang
//        |- eng
//        |- ger
//      |- midi
//        |- example.midi
//        |- ...
//
/* ------------------------------------------------- */

#include "DataManagement.h"
#include <Arduino.h>
#include <SD.h>
#include <Song.h>
#include <Language.h>


DataManagement::DataManagement(int pinCS)
{
  _pinCS = pinCS;
  _songCount = 0;
}

char DataManagement::begin()
{
  if (SD.begin(_pinCS))
  {
    return 1;
  } else {
    return 0;
  }
}

char DataManagement::parseSongInfos()
{
  File midiDir;
  Song *temp;
  boolean allFiles = true;
  uint16_t countMissing = 0;
  int error = 0;

  midiDir = SD.open("/midi/");
  if(!midiDir)
  {
    return _RETURNVAL_MIDI_DIR_NOT_FOUND;
  }

  Serial.println("");
  Serial.println("---------------- parseSongInfos() ----------------");

  while(true)
  {
    _midiFile = midiDir.openNextFile();

    if(! _midiFile) break; // no more files

    if(!_midiFile.isDirectory())   // ignore directories
    {
      if(_songCount < _MAX_SONG_COUNT) // only parse until _MAX_SONG_COUNT
      {
        if(_midiFile.name()[0] != '_' && String(_midiFile.name()).indexOf(".MID") >= 0 ) // only read in propper midi files
        {
          Serial.println("Song #"+String(_songCount,DEC)+": "+String(_midiFile.name()) + " : " + String(_midiFile.size(), DEC));

          temp = new Song();
          _midiFileHandler = new MidiFileHandler();

          temp->setPath("/midi/"+String(_midiFile.name()));
          temp->setTitle("Song #"+String(_songCount,DEC));

          // parse song info
          _midiFileHandler->setMidiFile(&_midiFile);
          error = _midiFileHandler->getHeaderInfo();

          switch(error)
          {
            case _MIDI_FILE_HANDLER_ERROR_HEADER_TITLE:
              Serial.println("incorrect header title");
              break;
            case _MIDI_FILE_HANDLER_ERROR_HEADER_LENGTH:
              Serial.println("incorrect header length");
              break;
            case _MIDI_FILE_HANDLER_ERROR_HEADER_FORMAT:
              Serial.println("incorrect MIDI Format");
              break;
            case _MIDI_FILE_HANDLER_ERROR_HEADER_TRACKNUM:
              Serial.println("corrupted midi file, incorrect track number");
              break;
            case _MIDI_FILE_HANDLER_ERROR_HEADER_SMPTE:
              Serial.println("currently, smpte based clocks are not supported.");
              break;
            default:
              Serial.println("Header is okay");

              // store data from header
              temp->setFormat(_midiFileHandler->getFormat());
              temp->setNtrks(_midiFileHandler->getNtrks());
              temp->setTpqn(_midiFileHandler->getTpqn());
              temp->setUsPerMidiQn(_midiFileHandler->getUsPerMidiQn());
          }

          if(_midiFileHandler->getFormat() == 0 && error == 0) // only read track when midi format 0 and no errors
          {

            error = _midiFileHandler->getTrackInfo();

            switch (error)
            {
              case _MIDI_FILE_HADLER_ERROR_TRACK_TITLE:
                Serial.println("incorrect track title");
                break;
              default:
                Serial.println("Track Info is okay");

                // store data from Song Info
                if(_midiFileHandler->getTitle().length() > 0)
                  temp->setTitle(_midiFileHandler->getTitle());
                temp->setLength(_midiFileHandler->getTotalTime());
                // get more info
            }
          }

          if(!error)
          {
            _songs[_songCount] = temp;
            _songCount++;
          } else {
            Serial.println("This file is corrupt: "+String(_midiFile.name()));
          }
        }

      } else {
        if(_midiFile.name()[0] != '_' && String(_midiFile.name()).indexOf(".MID") >= 0 )
        {
          allFiles = false;
          countMissing++;
          Serial.println("NOT STORED: "+String(_midiFile.name()) + " : " + String(_midiFile.size(), DEC));
        }
      }
    }
    _midiFile.close();
  }

  Serial.println("SongCount: "+String(_songCount,DEC));
  if(!allFiles)
  {
    Serial.println("Not all files could be stored. "+String(countMissing,DEC)+" files are missing.");
  }

  midiDir.close();

  for(uint16_t i=0; i < _songCount; i++)
  {
    Serial.println(_songs[i]->getInfo());
  }

  Serial.println("---------------------- END -----------------------");
  Serial.println("");

  return 0;
}

char DataManagement::startSong(int16_t selection)
{
  _midiFile = SD.open(_songs[selection]->getPath());
  if(!_midiFile)
  {
    return _RETURNVAL_MIDI_FILE_NOT_FOUND;
  }

  _midiFileHandler = new MidiFileHandler();

  _midiFileHandler->setMidiFile(&_midiFile);
  _midiFileHandler->setPlaying(true);
  _midiFileHandler->getHeaderInfo(); // to move to position
  _midiFileHandler->startSong();

  return 0;
}

char DataManagement::playSong()
{
  return _midiFileHandler->playSong();
}

void DataManagement::stopSong()
{
  _midiFileHandler->stopSong();
  _midiFile.close();
}

Song DataManagement::getSong(int pos)
{
  return *_songs[pos];
}

String DataManagement::getTranslations(String languageCode)
{
  return _translations->getTranslationNew(languageCode);
}

uint16_t DataManagement::getSongCount()
{
  return _songCount;
}

uint32_t DataManagement::getCurrentTime()
{
  return _midiFileHandler->getTotalTime();
}

char DataManagement::changeLanguage(char language)
{
  _language = language;
  File languageFile;

  switch (language)
  {
    case _languageEnglish:
      if(!SD.exists("lang/eng"))
      {
        return -1;
      } else {
        languageFile = SD.open("lang/eng");
      }
      break;
    case _languageGerman:
      if(!SD.exists("lang/ger"))
      {
        return -1;
      } else {
        languageFile = SD.open("lang/ger");
      }
      break;
    default:
      // error HOW TO HANLDE??
      return -1;
      break;
  }
  _translations->parseLanguageFile(languageFile);

  languageFile.close();
  return 0;
}

char DataManagement::getLanguageFile(char language, File &languageFile)
{
  switch (language)
  {
    case _languageEnglish:
      if(!SD.exists("lang/eng"))
      {
        return 0;
      } else {
        languageFile = SD.open("lang/eng");
        return 1;
      }
      break;
    case _languageGerman:
      if(!SD.exists("lang/ger"))
      {
        return 0;
      } else {
        languageFile = SD.open("lang/ger");
        return 1;
      }
      break;
    default:
      // error HOW TO HANLDE??
      return -1;
      break;
  }
  return 0;
}
