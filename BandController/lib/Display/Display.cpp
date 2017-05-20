/* ------------------------------------------------- */
//
//      BandController - Class to manage Display
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#include "Display.h"
#include <Arduino.h>
#include <avr/pgmspace.h>

// Bitmap information for icons
const unsigned char songsIcon [] PROGMEM = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00,
 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00,
 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfe, 0x78, 0x00,
 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x78, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x01, 0xf8, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x07, 0x80, 0x07, 0xff, 0xf8, 0x00,
 0x00, 0x00, 0x07, 0x80, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xdf, 0xff, 0xff, 0xf8, 0x00,
 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x00,
 0x00, 0x00, 0x07, 0xff, 0xff, 0xf0, 0x78, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0xff, 0x78, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x07, 0x80, 0x07, 0xff, 0xf8, 0x00,
 0x00, 0x00, 0x07, 0x80, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xc7, 0x80, 0x1f, 0xff, 0xf8, 0x00,
 0x00, 0x1f, 0xff, 0x80, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x7f, 0xff, 0x80, 0x3f, 0xff, 0xf8, 0x00,
 0x00, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xf0, 0x00,
 0x03, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xf0, 0x00, 0x07, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xe0, 0x00,
 0x07, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x00, 0x07, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0x00,
 0x07, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x3f, 0xfe, 0x00, 0x00,
 0x07, 0xff, 0xfc, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x07, 0xff, 0xfc, 0x00, 0x03, 0x80, 0x00, 0x00,
 0x07, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char settingsIcon [] PROGMEM = {
 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x1c, 0x03, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x03, 0xc0, 0x7c, 0x00, 0x00,
 0x00, 0x00, 0x3e, 0x07, 0xe0, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00,
 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00,
 0x00, 0x30, 0x7f, 0xff, 0xff, 0xfe, 0x0c, 0x00, 0x00, 0x78, 0xff, 0xff, 0xff, 0xff, 0x1e, 0x00,
 0x00, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0x00,
 0x18, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0x18, 0x3f, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xfc,
 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfc, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xfc,
 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xe0,
 0x07, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xe0,
 0x07, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xe0,
 0x07, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xf0,
 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff,
 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff,
 0x0f, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xe0,
 0x07, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xe0,
 0x07, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xe0,
 0x07, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0xf8,
 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfc,
 0x3f, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xfc, 0x18, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0x18,
 0x00, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x00,
 0x00, 0x78, 0xff, 0xff, 0xff, 0xff, 0x1e, 0x00, 0x00, 0x30, 0x7f, 0xff, 0xff, 0xfe, 0x0c, 0x00,
 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00,
 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x07, 0xe0, 0x7c, 0x00, 0x00,
 0x00, 0x00, 0x3e, 0x03, 0xc0, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x03, 0xc0, 0x38, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00
};



Display::Display(int pinCS, int pinDC, int pinRST, int pinBACKLIGHT, unsigned char backlight, int pinSdCS, unsigned char language):_display(pinCS, pinDC, pinRST), _dataManager(pinSdCS)
{
  _pinCS = pinCS;
  _pinDC = pinDC;
  _pinRST = pinRST;
  _pinSdCS = pinSdCS;
  _pinBACKLIGHT = pinBACKLIGHT;
  _backlight = backlight;
  _language = language;

  pinMode(_pinBACKLIGHT, OUTPUT);
  changeBrightness(_backlight);
}

void Display::begin()
{
  _display.begin();
  _display.setRotation(3);
  _display.fillScreen(_COLOR_DARK_GREY);
}

void Display::changeBrightness(unsigned char backlight)
{
  _backlight = backlight;
  analogWrite(_pinBACKLIGHT, (_backlight*255)/100);
}

void Display::changeLanguage(unsigned char language)
{
  _language = language;
}

void Display::displayErrorMessage(char errorCode, String errorMessage)
{
  uint8_t marginX = 20;
  uint8_t marginY = 50;
  uint8_t headerHight = 35;

  //_display.fillScreen(_COLOR_DARK_GREY);
  _display.fillRect(marginX, marginY, (_display.width()-(2*marginX)), (_display.height()-(2*marginY)), _COLOR_RED);
  centerText("Error #"+String(errorCode,DEC), 2, _COLOR_OFF_WHITE, marginX, marginY, (_display.width()-(2*marginX)), headerHight);
  _display.drawLine(marginX, marginY + headerHight, _display.width() - marginX, marginY + headerHight, _COLOR_OFF_WHITE);
  centerText(errorMessage, 2, _COLOR_OFF_WHITE, marginX, marginY+headerHight, (_display.width()-(2*marginX)), (_display.height()-(2*marginY))-headerHight);
}

void Display::displayBootupScreen()
{
  bmpDraw("/grfx/Logo.bmp",75,25,_display);
  changeBootupInfo("Bootup");
}

void Display::changeBootupInfo(String text)
{
  _display.fillRect(0, 210, _display.width(), 8, _COLOR_DARK_GREY);
  centerText(text,1,_COLOR_LIGHT_GREY,0,210,_display.width());
}

void Display::displayHomeScreen()
{
  createHeader(_translation.getTranslation("home_title", _language));
  _display.fillRect(0, _HEADER_HEIGHT, _display.width(), _display.height() - _HEADER_HEIGHT - _FOOTER_HEIGHT, _COLOR_OFF_WHITE);
  createFooter(_translation.getTranslation("footer", _language));
  // create buttons
  changeHomeScreenSelection(0);
}

void Display::changeHomeScreenSelection(char selection)
{
  createHomeScreenButtons(selection, _translation.getTranslation("songs_title", _language), _translation.getTranslation("settings_title", _language));
}


void Display::displaySettings()
{
  createHeader(_translation.getTranslation("settings_title", _language));
  _display.fillRect(0, _HEADER_HEIGHT, _display.width(), _display.height() - _HEADER_HEIGHT - _FOOTER_HEIGHT, _COLOR_OFF_WHITE);
  createFooter(_translation.getTranslation("footer", _language));
  // create buttons
  changeSettingsSelection(0);
}

void Display::changeSettingsSelection(char selection)
{
  createSettingsItem(0, _translation.getTranslation("language_label", _language), _translation.getTranslation("language_code", _language), selection == _SETTINGS_LANGUAGE);
  createSettingsItem(1, _translation.getTranslation("brightness_label", _language), String(_backlight)+"%", selection == _SETTINGS_BRIGHTNESS);
  createSettingsItem(2, _translation.getTranslation("return_label", _language), "", selection == _SETTINGS_RETURN);
}

void Display::changeSettingsValue(char selection, int valueChange)
{
  uint16_t y = _HEADER_HEIGHT + 10 + (_ITEM_HEIGHT * selection);
  String value = "NA";

  _display.fillRect(_display.width()-35-80 , y, 90, _ITEM_HEIGHT, _COLOR_RED);
  switch (selection)
  {
    case _SETTINGS_LANGUAGE:
      value = _translation.getTranslation("language_code", valueChange);
      break;
    case _SETTINGS_BRIGHTNESS:
      value = String(valueChange)+"%";
  }
  centerTextVerticallyAllignRight(value, 2, _COLOR_OFF_WHITE, _display.width()-35-80, y, 80, _ITEM_HEIGHT);
}

// Helper functions

void Display::centerText(String text, char fontSize, uint16_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  uint8_t fontWidth = fontSize * 6; // 6 is standard font width
  uint8_t fontHeight = fontSize * 8; // 8 is standard font height
  uint8_t length = text.length();

  if(height == 0)
    height = fontHeight;
  if(width == 0)
    width = fontWidth * length;

  if((length * fontWidth) <= width && fontHeight <= height)
  {
    _display.setCursor(x + ((width-(length*fontWidth))/2) + (fontSize/2), y + ((height-fontHeight)/2) + (fontSize/2)); //  - (fontSize/2) -> because of the empty spacing pixel in the character
  } else if((length * fontWidth) > width) {
    _display.setCursor(x - (((length*fontWidth)-width)/2) + (fontSize/2), y + ((height-fontHeight)/2) + (fontSize/2));
  } else if(fontHeight > height) {
    _display.setCursor(x + ((width-(length*fontWidth))/2) + (fontSize/2), y - ((fontHeight-height)/2) + (fontSize/2));
  } else {
    _display.setCursor(x - (((length*fontWidth)-width)/2) + (fontSize/2), y - ((fontHeight-height)/2) + (fontSize/2));
  }

  _display.setTextSize(fontSize);
  _display.setTextColor(color);
  _display.println(text);
}

void Display::centerTextVerticallyAllignLeft(String text, char fontSize, uint16_t color, uint16_t x, uint16_t y, uint16_t height)
{
  uint8_t fontHeight = fontSize * 8; // 8 is standard font height

  if(height == 0)
    height = fontHeight;

  if(fontHeight <= height)
  {
    _display.setCursor(x, y + ((height-fontHeight)/2) + (fontSize/2)); //  - (fontSize/2) -> because of the empty spacing pixel in the character
  } else if(fontHeight > height) {
    _display.setCursor(x, y - ((fontHeight-height)/2) + (fontSize/2));
  }

  _display.setTextSize(fontSize);
  _display.setTextColor(color);
  _display.println(text);
}

void Display::centerTextVerticallyAllignRight(String text, char fontSize, uint16_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  uint8_t fontWidth = fontSize * 6; // 6 is standard font width
  uint8_t fontHeight = fontSize * 8; // 8 is standard font height
  uint8_t length = text.length();

  if(height == 0)
    height = fontHeight;
  if(width == 0)
    width = fontWidth * length;

  if(fontHeight <= height)
  {
    _display.setCursor(x + width - (fontWidth * length), y + ((height-fontHeight)/2) + (fontSize/2)); //  - (fontSize/2) -> because of the empty spacing pixel in the character
  } else if(fontHeight > height) {
    _display.setCursor(x + width - (fontWidth * length), y - ((fontHeight-height)/2) + (fontSize/2));
  }

  _display.setTextSize(fontSize);
  _display.setTextColor(color);
  _display.println(text);
}

void Display::createHeader(String title)
{
  _display.fillRect(0, 0, _display.width(), _HEADER_HEIGHT, _COLOR_DARK_GREY);
  centerText("[ "+title+" ]", 2, _COLOR_OFF_WHITE, 0, 0, _display.width(), _HEADER_HEIGHT);
}

void Display::createFooter(String text)
{
  _display.fillRect(0, (_display.height()-_FOOTER_HEIGHT), _display.width(), _FOOTER_HEIGHT, _COLOR_LIGHT_GREY);
  centerText(text, 1, _COLOR_OFF_WHITE, 0, (_display.height()-_FOOTER_HEIGHT), _display.width(), _FOOTER_HEIGHT);
}

void Display::createHomeScreenButtons(char selection, String label1, String label2)
{
  uint16_t posX1 = ((_display.width() / 2) - 100) / 2;
  uint16_t posX2 = posX1 + (_display.width() / 2);

  centerText(label1, 2, ((!selection%2) ? _COLOR_RED : _COLOR_LIGHT_GREY), 0, _HEADER_HEIGHT + 130, _display.width()/2, 40);
  centerText(label2, 2, ((selection%2) ? _COLOR_RED : _COLOR_LIGHT_GREY), _display.width()/2, _HEADER_HEIGHT + 130, _display.width()/2, 40);

  // rounded rectangles create artifacts while creating
  //_display.fillRoundRect(posX1, _HEADER_HEIGHT + 30, 100, 100, 10, ((!selection%2) ? _COLOR_RED : _COLOR_LIGHT_GREY));
  //_display.fillRoundRect(posX2, _HEADER_HEIGHT + 30, 100, 100, 10, ((selection%2) ? _COLOR_RED : _COLOR_LIGHT_GREY));

  _display.fillRect(posX1, _HEADER_HEIGHT + 30, 100, 100, ((!selection%2) ? _COLOR_RED : _COLOR_LIGHT_GREY));
  _display.fillRect(posX2, _HEADER_HEIGHT + 30, 100, 100, ((selection%2) ? _COLOR_RED : _COLOR_LIGHT_GREY));

  _display.drawBitmap(posX1+18, _HEADER_HEIGHT + 30+18, songsIcon, 64, 64, _COLOR_OFF_WHITE);
  _display.drawBitmap(posX2+18, _HEADER_HEIGHT + 30+18, settingsIcon, 64, 64, _COLOR_OFF_WHITE);
}

void Display::createSettingsItem(uint16_t pos, String label, String value, boolean selected)
{
  uint16_t y = _HEADER_HEIGHT + 10 + (_ITEM_HEIGHT * pos);

  _display.fillRect(15, y + ((_ITEM_HEIGHT-8)/2), 8, 8, selected ? _COLOR_RED : _COLOR_DARK_GREY);

  if(!selected)
    _display.fillRect(15+2, y + ((_ITEM_HEIGHT-4)/2), 4, 4, _COLOR_OFF_WHITE);

  centerTextVerticallyAllignLeft(label, 2, (selected) ? _COLOR_RED : _COLOR_DARK_GREY, 35, y, _ITEM_HEIGHT);
  _display.fillRect(_display.width()-35-80 , y, 90, _ITEM_HEIGHT, _COLOR_OFF_WHITE);
  centerTextVerticallyAllignRight(value, 2, (selected) ? _COLOR_RED : _COLOR_DARK_GREY, _display.width()-35-80, y, 80, _ITEM_HEIGHT);
  //_display.fillRect(_display.width()-15-80, y, 80 , _ITEM_HEIGHT, _COLOR_RED);
}

// ----------------- ADOPTED CODE ----------------- //
// The follwoing functions where adopted (and slightly modified)
// from Adafruits example code for the Adafruit_ILI9340 and can be found
// https://github.com/adafruit/Adafruit_ILI9340/blob/master/examples/spitftbitmap/spitftbitmap.ino
//
// Functions:
//  - bmpDraw(String filename, uint16_t x, uint16_t y, Adafruit_ILI9340 _display);
//  - uint16_t Display::read16(File & f);
//  - uint32_t Display::read32(File & f);

// This function opens a 24 Bit Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void Display::bmpDraw(String filename, uint16_t x, uint16_t y, Adafruit_ILI9340 _display)
{

  File     bmpFile;
  int      bmpWidth, bmpHeight;         // W+H in pixels
  uint8_t  bmpDepth;                    // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;              // Start of image data in file
  uint32_t rowSize;                     // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL];       // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer);  // Current position in sdbuffer
  boolean  goodBmp = false;             // Set to true on valid header parse
  boolean  flip    = true;              // BMP is sto_COLOR_RED bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= unsigned(_display.width())) || (y >= unsigned(_display.height()))) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if (!(bmpFile = SD.open(filename))) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= unsigned(_display.width()))  w = _display.width()  - x;
        if((y+h-1) >= unsigned(_display.height())) h = _display.height() - y;

        // Set TFT address window to clipped image bounds
        _display.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is sto_COLOR_RED bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is sto_COLOR_RED top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            _display.pushColor(_display.Color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is sto_COLOR_RED little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t Display::read16(File & f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t Display::read32(File & f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

// ----------------- END OF ADOPTED CODE ----------------- //

void Display::printTestScreen()
{
  _display.fillRect(0, 0, 64, 240, _COLOR_WHITE);   // White
  _display.fillRect(64, 0, 64, 240, _COLOR_OFF_WHITE);  // Off-White
  _display.fillRect(128, 0, 64, 240, _COLOR_LIGHT_GREY);   // light grey
  _display.fillRect(192, 0, 64, 240, _COLOR_DARK_GREY);    // dark grey
  _display.fillRect(256, 0, 64, 240, _COLOR_RED);   // RED

  _display.fillRect(27, 30, 10, 10, _COLOR_WHITE);   // White
  _display.fillRect(27, 70, 10, 10, _COLOR_OFF_WHITE);  // Off-White
  _display.fillRect(27, 110, 10, 10, _COLOR_LIGHT_GREY);   // light grey
  _display.fillRect(27, 150, 10, 10, _COLOR_DARK_GREY);    // dark grey
  _display.fillRect(27, 190, 10, 10, _COLOR_RED);   // RED

  _display.fillRect(91, 30, 10, 10, _COLOR_WHITE);   // White
  _display.fillRect(91, 70, 10, 10, _COLOR_OFF_WHITE);  // Off-White
  _display.fillRect(91, 110, 10, 10, _COLOR_LIGHT_GREY);   // light grey
  _display.fillRect(91, 150, 10, 10, _COLOR_DARK_GREY);    // dark grey
  _display.fillRect(91, 190, 10, 10, _COLOR_RED);   // RED

  _display.fillRect(155, 30, 10, 10, _COLOR_WHITE);   // White
  _display.fillRect(155, 70, 10, 10, _COLOR_OFF_WHITE);  // Off-White
  _display.fillRect(155, 110, 10, 10, _COLOR_LIGHT_GREY);   // light grey
  _display.fillRect(155, 150, 10, 10, _COLOR_DARK_GREY);    // dark grey
  _display.fillRect(155, 190, 10, 10, _COLOR_RED);   // RED

  _display.fillRect(219, 30, 10, 10, _COLOR_WHITE);   // White
  _display.fillRect(219, 70, 10, 10, _COLOR_OFF_WHITE);  // Off-White
  _display.fillRect(219, 110, 10, 10, _COLOR_LIGHT_GREY);   // light grey
  _display.fillRect(219, 150, 10, 10, _COLOR_DARK_GREY);    // dark grey
  _display.fillRect(219, 190, 10, 10, _COLOR_RED);   // RED

  _display.fillRect(283, 30, 10, 10, _COLOR_WHITE);   // White
  _display.fillRect(283, 70, 10, 10, _COLOR_OFF_WHITE);  // Off-White
  _display.fillRect(283, 110, 10, 10, _COLOR_LIGHT_GREY);   // light grey
  _display.fillRect(283, 150, 10, 10, _COLOR_DARK_GREY);    // dark grey
  _display.fillRect(283, 190, 10, 10, _COLOR_RED);   // RED
}
