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
#include <SD.h>
#include <Fonts/FreeMono12pt7b.h>

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
  uint16_t white = _display.Color565(255, 255, 255);
  //uint16_t offWhite = _display.Color565(229, 225, 209);
  uint16_t offWhite = _display.Color565(224, 213, 170); // new
  uint16_t lightGrey = _display.Color565(82, 97, 109);
  uint16_t darkGrey = _display.Color565(44, 52, 59);
  uint16_t red = _display.Color565(196, 71, 65);

  _display.begin();
  _display.setRotation(3);
  _display.fillScreen(darkGrey);
  /*
  _display.fillRect(0, 0, 64, 240, white);   // White
  _display.fillRect(64, 0, 64, 240, offWhite);  // Off-White
  _display.fillRect(128, 0, 64, 240, lightGrey);   // light grey
  _display.fillRect(192, 0, 64, 240, darkGrey);    // dark grey
  _display.fillRect(256, 0, 64, 240, red);   // RED

  _display.fillRect(27, 30, 10, 10, white);   // White
  _display.fillRect(27, 70, 10, 10, offWhite);  // Off-White
  _display.fillRect(27, 110, 10, 10, lightGrey);   // light grey
  _display.fillRect(27, 150, 10, 10, darkGrey);    // dark grey
  _display.fillRect(27, 190, 10, 10, red);   // RED

  _display.fillRect(91, 30, 10, 10, white);   // White
  _display.fillRect(91, 70, 10, 10, offWhite);  // Off-White
  _display.fillRect(91, 110, 10, 10, lightGrey);   // light grey
  _display.fillRect(91, 150, 10, 10, darkGrey);    // dark grey
  _display.fillRect(91, 190, 10, 10, red);   // RED

  _display.fillRect(155, 30, 10, 10, white);   // White
  _display.fillRect(155, 70, 10, 10, offWhite);  // Off-White
  _display.fillRect(155, 110, 10, 10, lightGrey);   // light grey
  _display.fillRect(155, 150, 10, 10, darkGrey);    // dark grey
  _display.fillRect(155, 190, 10, 10, red);   // RED

  _display.fillRect(219, 30, 10, 10, white);   // White
  _display.fillRect(219, 70, 10, 10, offWhite);  // Off-White
  _display.fillRect(219, 110, 10, 10, lightGrey);   // light grey
  _display.fillRect(219, 150, 10, 10, darkGrey);    // dark grey
  _display.fillRect(219, 190, 10, 10, red);   // RED

  _display.fillRect(283, 30, 10, 10, white);   // White
  _display.fillRect(283, 70, 10, 10, offWhite);  // Off-White
  _display.fillRect(283, 110, 10, 10, lightGrey);   // light grey
  _display.fillRect(283, 150, 10, 10, darkGrey);    // dark grey
  _display.fillRect(283, 190, 10, 10, red);   // RED
  */

  bmpDraw("/grfx/Logo.bmp",75,22,_display);

  _display.setCursor(((320-(18*6))/2), 215);
  _display.setTextColor(lightGrey);
  _display.setTextSize(1);
  _display.println("SD-Card Loading...");
}

void Display::setBacklight(unsigned char backlight)
{
  _backlight = backlight;
  analogWrite(_pinBACKLIGHT, _backlight);
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void Display::bmpDraw(char *filename, uint16_t x, uint16_t y, Adafruit_ILI9340 _display)
{

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= _display.width()) || (y >= _display.height())) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
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
        if((x+w-1) >= _display.width())  w = _display.width()  - x;
        if((y+h-1) >= _display.height()) h = _display.height() - y;

        // Set TFT address window to clipped image bounds
        _display.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
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
// BMP data is stored little-endian, Arduino is little-endian too.
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
