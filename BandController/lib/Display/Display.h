/* ------------------------------------------------- */
//
//      BandController - Class to manage Display
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#ifndef Display_h
#define Display_h

#include <Adafruit_ILI9340.h>
#include <SD.h>

class Display
{
  public:
    Display(int pinCS, int pinDC, int pinRST, int pinBACKLIGHT, unsigned char backlight);
    void begin();
    void setBacklight(unsigned char backlight);

  private:
    int _pinCS;
    int _pinDC;
    int _pinRST;
    int _pinBACKLIGHT;
    unsigned char _backlight;
    Adafruit_ILI9340 _display;
    uint32_t read32(File & f);
    uint16_t read16(File & f);
    void bmpDraw(char *filename, uint16_t x, uint16_t y, Adafruit_ILI9340 _display);
};

#endif
