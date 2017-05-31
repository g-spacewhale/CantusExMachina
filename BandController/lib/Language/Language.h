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
    String getTranslationNew(String languageCode);
    String getTranslation(String languageCode, uint8_t language);
    void parseLanguageFile(File &languageFile);

  private:
    uint8_t _language;
    uint16_t _translationsCount_NEW;

    String _languageCodes_NEW[_MAX_TRANSLATION_COUNT];
    String _translations_NEW[_MAX_TRANSLATION_COUNT];

    String _languageCode[_MAX_TRANSLATION_COUNT];
    String _translations[2][_MAX_TRANSLATION_COUNT];

    void createLanguageMap();
};

#endif
