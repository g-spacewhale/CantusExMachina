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

void Language::parseLanguageFile(File &languageFile)
{
  String languageCode;
  String translation;

  Serial.println("");
  Serial.println("parseLanguageFile");
  Serial.println("");

  for(int i = 0; i < _MAX_TRANSLATION_COUNT; i++)
  {
    if(languageFile.available() != 0)
    {
      /*text = "";
      while((temp = languageFile.read()) != '=')
      {
        text = text + char(temp);
      }
      //_languageCodes_NEW[0] = text;
      Serial.print(text+" = ");
      text = "";
      while((temp = languageFile.read()) != '\n')
      {
        text = text + char(temp);
      }
      //_translations_NEW[0] = text;
      Serial.println(text);*/

      languageCode = languageFile.readStringUntil('=');
      translation = languageFile.readStringUntil('\n');
      Serial.println("#"+String(i,DEC)+": \""+languageCode+"\" = \""+translation+"\"");
      _languageCodes_NEW[0] = languageCode;
      _translations_NEW[0] = translation;
      //_languageCodes_NEW[i] = languageFile.readStringUntil('=');
      //_translations_NEW[i] = languageFile.readStringUntil('\n');
      //Serial.println(_languageCodes_NEW[i] + " " + languageFile.readStringUntil('\n'));
      //_translationsCount_NEW = i;
    } else {
      break;
    }
  }
  Serial.println("");
  Serial.println("");
}

String Language::getTranslationNew(String languageCode)
{
  for(uint32_t i = 0; i < _translationsCount_NEW; i++)
  {
    if(_languageCodes_NEW[i].indexOf(languageCode) >= 0 )
      return _translations_NEW[i];
  }
  return "- NA -";
}

String Language::getTranslation(String languageCode, uint8_t language)
{
  for(uint16_t i = 0; i < _translationsCount; i++)
  {
    if(_languageCode[i].indexOf(languageCode) >= 0 )
      return _translations[language][i];
  }
  return "- NA -";
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
  _translations[_languageEnglish] [5] = "Return";
  _translations[_languageGerman]  [5] = "Zurueck";

  _languageCode                   [6] = "language_label";
  _translations[_languageEnglish] [6] = "Language:";
  _translations[_languageGerman]  [6] = "Sprache:";

  _languageCode                   [7] = "brightness_label";
  _translations[_languageEnglish] [7] = "Brightness:";
  _translations[_languageGerman]  [7] = "Helligkeit:";

  _languageCode                   [8] = "playing_title";
  _translations[_languageEnglish] [8] = "Playing Song";
  _translations[_languageGerman]  [8] = "Spielt Lied";

  _languageCode                   [9] = "play_label";
  _translations[_languageEnglish] [9] = "Play";
  _translations[_languageGerman]  [9] = "Abspielen";

  _languageCode                   [10] = "pause_label";
  _translations[_languageEnglish] [10] = "Pause";
  _translations[_languageGerman]  [10] = "Pause";

  _languageCode                   [11] = "stop_label";
  _translations[_languageEnglish] [11] = "Stop";
  _translations[_languageGerman]  [11] = "Stop";

  _languageCode                   [12] = "notSupported_label";
  _translations[_languageEnglish] [12] = "Not supported";
  _translations[_languageGerman]  [12] = "Nicht unterstuetzt";

  _languageCode                   [13] = "length_label";
  _translations[_languageEnglish] [13] = "Length";
  _translations[_languageGerman]  [13] = "Laenge";

  _languageCode                   [14] = "minute_label";
  _translations[_languageEnglish] [14] = "min";
  _translations[_languageGerman]  [14] = "min";

  _languageCode                   [15] = "language_code_0";
  _translations[_languageEnglish] [15] = "ENG";
  _translations[_languageGerman]  [15] = "ENG";

  _languageCode                   [16] = "language_code_1";
  _translations[_languageEnglish] [16] = "GER";
  _translations[_languageGerman]  [16] = "DEU";
}
