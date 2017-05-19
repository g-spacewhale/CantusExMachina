/* ------------------------------------------------- */
//
//        BandController - Class to manage the
//                   language files
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */


#include "Language.h"
#include <Arduino.h>
#include <SD.h>

Language::Language()
{
  _language = _languageEnglish;
  createLanguageMap();
}

String Language::getTranslation(String languageCode, uint8_t language)
{
  for(uint16_t i = 0; i < _translationsCount; i++)
  {
    if(_languageCode[i].indexOf(languageCode) >= 0 )
      return _translations[language][i];
  }
  return "NA";
}

void Language::createLanguageMap()
{
  _languageCode                   [0] = "home_title";
  _translations[_languageEnglish] [0] = "Menu";
  _translations[_languageGerman]  [0] = "Menü";

  _languageCode                   [1] = "songs_title";
  _translations[_languageEnglish] [1] = "Songs";
  _translations[_languageGerman]  [1] = "Lieder";

  _languageCode                   [2] = "settings_title";
  _translations[_languageEnglish] [2] = "Settings";
  _translations[_languageGerman]  [2] = "Einstellungen";
}
