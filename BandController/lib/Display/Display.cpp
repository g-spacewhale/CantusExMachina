/* ------------------------------------------------- */
//
//      BandController - Class to manage Display
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#include "Display.h"
#include "Arduino.h"

Display::Display(int pinCS, int pinDC, int pinRST, int pinBACKLIGHT):_display(pinCS, pinDC, pinRST)
{
  _pinCS = pinCS;
  _pinDC = pinDC;
  _pinRST = pinRST;
  _pinBACKLIGHT = pinBACKLIGHT;

  pinMode(pinBACKLIGHT, OUTPUT);
  analogWrite(pinBACKLIGHT, 128);
}


void Display::begin()
{
  _display.begin();
  _display.setRotation(3);
  _display.fillScreen(ILI9340_WHITE);
  _display.setCursor(0, 0);
  _display.setTextColor(ILI9340_BLACK);
  _display.setTextSize(1);
  _display.println("Cantus Ex Machina");
}
