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

#define _languageEnglish 0    // english is the standard language
#define _languageGerman 1

#include <Arduino.h>
#include <SD.h>

class Language
{
  public:
    Language();
    String getTranslation(File &languageFile, String languageCode);

  private:
};

#endif
