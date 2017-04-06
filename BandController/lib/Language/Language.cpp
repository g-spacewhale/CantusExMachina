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

char Language::createLanguageMap(File &languageFile)
{

  //languageFile.readStringUntil('=');
  return 1;
}

String Language::getTranslation(String languageCode)
{
  return "1";
}
