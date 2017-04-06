/* ------------------------------------------------- */
//
//      BandController - Class to manage Display
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#include "Display.h"
#include <Arduino.h>
#include <Adafruit_ILI9340.h>

Display::Display(int pinCS, int pinDC, int pinRST, int pinBACKLIGHT, unsigned char backlight):_display(pinCS, pinDC, pinRST)
{
  _pinCS = pinCS;
  _pinDC = pinDC;
  _pinRST = pinRST;
  _pinBACKLIGHT = pinBACKLIGHT;
  _backlight = backlight;

  pinMode(_pinBACKLIGHT, OUTPUT);
  analogWrite(pinBACKLIGHT, _backlight);
}

void Display::begin()
{
  _display.begin();
  _display.setRotation(1);
  _display.fillScreen(ILI9340_WHITE);
  _display.setCursor(0, 0);
  _display.setTextColor(ILI9340_BLACK);
  _display.setTextSize(1);
  _display.println("Cantus Ex Machina");
}

void Display::setBacklight(unsigned char backlight)
{
  _backlight = backlight;
  analogWrite(_pinBACKLIGHT, _backlight);
}
