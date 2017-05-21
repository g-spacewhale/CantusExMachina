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
  File entry;
  Song *temp;
  boolean allFiles = true;
  uint16_t countMissing = 0;

  midiDir = SD.open("/midi/");
  if(!midiDir)
  {
    return _RETURNVAL_MIDI_DIR_NOT_FOUND;
  }


  Serial.println("");
  Serial.println("---------------- parseSongInfos() ----------------");

  while(true)
  {
    entry = midiDir.openNextFile();

    if(! entry) break; // no more files

    if(!entry.isDirectory())   // ignore directories
    {
      if(_songCount < _MAX_SONG_COUNT) // only parse until _MAX_SONG_COUNT
      {
        if(entry.name()[0] != '_' && String(entry.name()).indexOf(".MID") >= 0 ) // only read in propper midi files
        {
          Serial.println("Song #"+String(_songCount,DEC)+": "+String(entry.name()) + " : " + String(entry.size(), DEC));

          temp = new Song();

          temp->setPath("/midi/"+String(entry.name()));
          temp->setTitle(String(entry.name()));
          temp->setFormat((_songCount%2) ? 1 : 0);
          temp->setLength(entry.size());

          _songs[_songCount] = temp;

          _songCount++;
        }

      } else {
        if(entry.name()[0] != '_' && String(entry.name()).indexOf(".MID") >= 0 )
        {
          allFiles = false;
          countMissing++;
          Serial.println("NOT STORED: "+String(entry.name()) + " : " + String(entry.size(), DEC));
        }
      }
    }
    entry.close();
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


Song DataManagement::getSong(int pos)
{
  return *_songs[pos];
}

uint16_t DataManagement::getSongCount()
{
  return _songCount;
}

/*char DataManagement::getLanguageFile(char language, File &languageFile)
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
}*/
