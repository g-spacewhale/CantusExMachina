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

Language::Language(){}

/*char Language::createLanguageMap(File &languageFile)
{
  uint16_t translationsCount = 0;
  String languageCode;
  String translation;

  while (languageFile.available())
  {
    languageCode = languageFile.readStringUntil('=');
    translation = languageFile.readStringUntil('\n');

    _languageCode[translationsCount] = languageCode;
    _translation[translationsCount] = translation;

    Serial.println(_languageCode[translationsCount]+" = "+_translation[translationsCount]);

    translationsCount++;

    if(translationsCount > _MAX_NUM_TRANSLATIONS) // to many translations
      return 0;
  }

  _translationsCount = translationsCount;

  Serial.println("Number of Translations: "+String(translationsCount,DEC));

  return 1;
} */

String Language::getTranslation(File &languageFile, String languageCode)
{
  String _languageCode;
  String _translation;

  while (languageFile.available())
  {
    _languageCode = languageFile.readStringUntil('=');
    _translation = languageFile.readStringUntil('\n');

    if(_languageCode.indexOf(languageCode) >= 0 )
      return _translation;
  }

  return "NA";
}

/*String Language::getTranslation(String languageCode)
{
  for(uint16_t i = 0; i < _translationsCount; i++)
  {
    if(_languageCode[i].compareTo(languageCode) == 0)
      return _translation[i];
  }
  return "NULL";
}*/
