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

#define _LANGUAGE_COUNT 2
#define _languageEnglish 0    // english is the standard language
#define _languageGerman 1

#define _translationsCount 8

#include <Arduino.h>
#include <SD.h>

class Language
{
  public:
    Language();
    //String getTranslation(File &languageFile, String languageCode);
    String getTranslation(String languageCode, uint8_t language);

  private:
    uint8_t _language;
    String _languageCode[_translationsCount];
    String _translations[2][_translationsCount];

    void createLanguageMap();
};

#endif
