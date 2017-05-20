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

  _languageCode                   [0] = "language_code";
  _translations[_languageEnglish] [0] = "ENG";
  _translations[_languageGerman]  [0] = "DEU";

  _languageCode                   [1] = "home_title";
  _translations[_languageEnglish] [1] = "Menu";
  _translations[_languageGerman]  [1] = "Menue";

  _languageCode                   [2] = "songs_title";
  _translations[_languageEnglish] [2] = "Songs";
  _translations[_languageGerman]  [2] = "Lieder";

  _languageCode                   [3] = "settings_title";
  _translations[_languageEnglish] [3] = "Settings";
  _translations[_languageGerman]  [3] = "Einstellungen";

  _languageCode                   [4] = "footer";
  _translations[_languageEnglish] [4] = "Rotate to select | Click to confirm";
  _translations[_languageGerman]  [4] = "Drehen: Auswaehlen | Klicken: Bestaetigen";

  _languageCode                   [5] = "return_label";
  _translations[_languageEnglish] [5] = "< Return";
  _translations[_languageGerman]  [5] = "< Zurueck";

  _languageCode                   [6] = "language_label";
  _translations[_languageEnglish] [6] = "Language:";
  _translations[_languageGerman]  [6] = "Sprache:";

  _languageCode                   [7] = "brightness_label";
  _translations[_languageEnglish] [7] = "Brightness";
  _translations[_languageGerman]  [7] = "Helligkeit";


}
