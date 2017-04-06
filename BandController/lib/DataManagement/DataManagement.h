/* ------------------------------------------------- */
//
//      BandController - Class to manage the SD Data
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#ifndef DataManagement_h
#define DataManagement_h

#include <SD.h>

#define _languageEnglish 0    // english is the standard language
#define _languageGerman 1

class DataManagement
{
  public:
    DataManagement(int pinCS);
    char begin();
    char getLanguageFile(char language, File &languageFile);
  private:
    void printDirectory(File dir, int numTabs);
    int _pinCS;
};

#endif
