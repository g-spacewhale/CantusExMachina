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

#define _MAX_TRANSLATION_COUNT 20
#define _translationsCount 17

#include <Arduino.h>
#include <SD.h>

class Language
{
  public:
    Language();
    String getTranslation(String languageCode, uint8_t language);

  private:
    uint8_t _language;

    String _languageCode[_MAX_TRANSLATION_COUNT];
    String _translations[2][_MAX_TRANSLATION_COUNT];

    void createLanguageMap();
};

#endif
