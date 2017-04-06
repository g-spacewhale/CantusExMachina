/* ------------------------------------------------- */
//
//        BandController - Class to manage the
//                   language files
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#ifndef Language_h
#define Language_h

#include <Arduino.h>
#include <SD.h>

class Language
{
  public:
    Language();
    char createLanguageMap(File &languageFile);
    String getTranslation(String languageCode);

  private:
    String _languageCode[];
    String _translation[];
};

#endif
