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

class Display
{
  public:
    Display(int pinCS, int pinDC, int pinRST, int pinBACKLIGHT);
    void begin();

  private:
    int _pinCS;
    int _pinDC;
    int _pinRST;
    int _pinBACKLIGHT;
    Adafruit_ILI9340 _display;
};

#endif
