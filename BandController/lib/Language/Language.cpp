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
