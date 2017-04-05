/* ------------------------------------------------- */
//
//             BandController - Main Class
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

// libraries
#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>

// custom classes and additional headers for information
#include <StateDefines.h>
#include <Display.h>

#define _DEBUGING   // defines if debugg messages are sent via serial (-> comment out if no messages shoul be sent)
#ifdef _DEBUGING
  #define SEND_DEBUG_MESSAGE(x,y) sendDebugMessage(x,y)
#else
 #define SEND_DEBUG_MESSAGE(x,y)
#endif

#define _FIRMWARE_VERSION 0.01

// EEPROM Cell codes (defines in which EEPROM Cell which information is stored)
#define _EEPROM_LANGUAGE 0
#define _EEPROM_DISPLAY_BRIGHTNESS 1

#define _languageEnglish 0    // english is the standard language
#define _languageGerman 1

// ------------------ Defining of Pin mapping ------------------
// defines the 3 Data-Pins of the Roatary Encoder
#define _encoderPinA 2
#define _encoderPinB 3
#define _encoderPinClick 18

// defines the 4 Data-Pins of the Display
#define _displayPinCS 53
#define _displayPinDC 9
#define _displayPinRST 8
#define _displayPinBACKLIGHT 4


// ------------------- Variable declarations -------------------
// Settings
unsigned char _language = _languageEnglish;
unsigned char _displayBrightness = 255;

volatile char _encoderCounterTurns = 0;
volatile unsigned char _encoderCounterClicks = 0;

char counterTurns = 0;
char counterClicks = 0;

unsigned char _state = _STATE_BOOTUP;       // holds the current state of the statemachine - refere to 'Defining of States'
unsigned char _previousState = 0;           // holds the previous state of the statemachine

Display _display = Display(_displayPinCS, _displayPinDC, _displayPinRST, _displayPinBACKLIGHT, _displayBrightness);


// ------------------- Forward declarations --------------------
void sendDebugMessage(unsigned char messageCode, String message);

void encoderInterruptTurn();
void encoderInterruptClick();
char encoderGetTurns();
unsigned char encoderGetClicks();
void encoderResetTurns();
void encoderResetClicks();

void changeState(unsigned char newState);
void bootUpRoutine();

void setup()
{
    // setup serial for debugg messages
    #ifdef _DEBUGING
      Serial.begin(9600);
      Serial.println("Debug Messages activated:");
    #endif

    pinMode(_encoderPinA, INPUT);
    pinMode(_encoderPinB, INPUT);
    pinMode(_encoderPinClick, INPUT);

    attachInterrupt(digitalPinToInterrupt(_encoderPinA), encoderInterruptTurn, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_encoderPinClick), encoderInterruptClick, RISING);
}

// Statemachine Loop
void loop()
{
  switch (_state)
  {
    case _STATE_BOOTUP:
      bootUpRoutine();
      break;
    case _STATE_HOME:

      // encoder Testing
      if(encoderGetTurns())
      {
        counterTurns += encoderGetTurns();
        encoderResetTurns();
        SEND_DEBUG_MESSAGE(0, "counterTurns = "+String(counterTurns, DEC));
      }
      if(encoderGetClicks())
      {
        counterClicks += encoderGetClicks();
        encoderResetClicks();
        SEND_DEBUG_MESSAGE(0, "counterClicks = "+String(counterClicks, DEC));
      }

      break;
    default:
      //Error message
      SEND_DEBUG_MESSAGE(0, "ERROR: Statemachine went into default case");
      break;
  }
}

void sendDebugMessage(unsigned char messageCode, String message)
{
  Serial.print("DebuggMessage(#");
  Serial.print(messageCode);
  Serial.print("):");
  Serial.println(message);
}

void encoderInterruptTurn()
{
  if (digitalRead(_encoderPinA) == HIGH)    // found a low-to-high on channel A
  {
    if (digitalRead(_encoderPinB) == LOW)   // check channel B to see which way
      _encoderCounterTurns--;                // encoder is turning: CCW
    else
      _encoderCounterTurns++;                // encoder is turning:CW
  } else {                                  // found a high-to-low on channel A
    if (digitalRead(_encoderPinB) == LOW)   // check channel B to see which way
      _encoderCounterTurns++;                // encoder is turning:CW
    else
      _encoderCounterTurns--;                // encoder is turning:CCW
  }
}

void encoderInterruptClick()
{
  _encoderCounterClicks++;
}

char encoderGetTurns()
{
  return _encoderCounterTurns;
}

unsigned char encoderGetClicks()
{
  return _encoderCounterClicks;
}

void encoderResetTurns()
{
  _encoderCounterTurns = 0;
}

void encoderResetClicks()
{
  _encoderCounterClicks = 0;
}

void changeState(unsigned char newState)
{
  _previousState = _state;
  _state = newState;
  SEND_DEBUG_MESSAGE(0, "Changed State");
}

void bootUpRoutine()
{
  // Load Settings from EEPROM
  _language = EEPROM.read(_EEPROM_LANGUAGE);
  SEND_DEBUG_MESSAGE(0, "Language from EEPROM = "+String(_language, DEC));
  _displayBrightness = EEPROM.read(_EEPROM_DISPLAY_BRIGHTNESS);
  SEND_DEBUG_MESSAGE(0, "Display Brightness from EEPROM = "+String(_displayBrightness, DEC));

  // Init Display
  _display.begin();

  // Init SD-Card

  // Load infos from SD card

  // When all done:
  changeState(_STATE_HOME);
}
