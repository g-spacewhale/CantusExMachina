/* ------------------------------------------------- */
//
//      BandController - Class to manage Display
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#ifndef Display_h
#define Display_h

#define _COLOR_WHITE 0xffff       // 255, 255, 255
#define _COLOR_OFF_WHITE 0xe6b5   // 224, 213, 170 (original: 229, 225, 209)
#define _COLOR_LIGHT_GREY 0x530d  // 82, 97, 109
#define _COLOR_DARK_GREY 0x29a7   // 44, 52, 59
#define _COLOR_RED 0xc228         // 196, 71, 65

#define _HEADER_HEIGHT 30
#define _FOOTER_HEIGHT 20

#include <Adafruit_ILI9340.h>
#include <SD.h>
#include <Language.h>
#include <DataManagement.h>


class Display
{
  public:
    Display(int pinCS, int pinDC, int pinRST, int pinBACKLIGHT, unsigned char backlight, int pinSdCS, unsigned char language);
    void begin();
    void setBacklight(unsigned char backlight);
    void displayErrorMessage(char errorCode, String errorMessage);
    void displayBootupScreen();
    void changeBootupInfo(String text);
    void displayHomeScreen();
    void changeHomeScreenSelection(char selection);
    void changeLanguage(unsigned char language);

  private:
    int _pinCS;
    int _pinDC;
    int _pinRST;
    int _pinBACKLIGHT;
    int _pinSdCS;
    unsigned char _backlight;
    unsigned char _language;
    Adafruit_ILI9340 _display;
    DataManagement _dataManager;
    Language _translation;
    File _languageFile;
    uint32_t read32(File & f);
    uint16_t read16(File & f);

    // Helper functions
    void centerText(String text, char fontSize, uint16_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height = 0);
    void createHeader(String title);
    void createFooter(String text);
    void createHomeScreenButtons(char selection, String label1, String label2);

    String getTranslation(String languageCode);
    void printTestScreen();

    void bmpDraw(String filename, uint16_t x, uint16_t y, Adafruit_ILI9340 _display);
};

#endif
