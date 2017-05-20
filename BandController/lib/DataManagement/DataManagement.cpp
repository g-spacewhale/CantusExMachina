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
  Song *temp;
  for(int i=0; i < _MAX_SONG_COUNT; i++)
  {
    temp = new Song("/midi/"+String(i,DEC)+".mid","Song"+String(i,DEC),1,1);
    _songs[i] = temp;
  }

  for(int i=0; i < _MAX_SONG_COUNT; i++)
  {
    Serial.println(_songs[i]->getInfo());
  }

  return 0;
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
