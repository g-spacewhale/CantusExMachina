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

// EEPROM Cell codes
#define _EEPROM_DISPLAY_BRIGHTNESS 0
#define _EEPROM_LANGUAGE 1

// ------------------ Defining of Pin mapping ------------------
// defines the 3 Data-Pins of the Roatary Encoder
#define _encoderPinA 2
#define _encoderPinB 3
#define _endocerPinClick 18

// defines the 4 Data-Pins of the Display
#define _displayPinCS 53
#define _displayPinDC 9
#define _displayPinRST 8
#define _displayPinBACKLIGHT 4


// ------------------- Variable declarations -------------------
unsigned char _state = _STATE_BOOTUP;       // holds the current state of the statemachine - refere to 'Defining of States'
unsigned char _previousState = 0;           // holds the previous state of the statemachine

Display _display = Display(_displayPinCS, _displayPinDC, _displayPinRST, _displayPinBACKLIGHT);

// forward declarations
void sendDebugMessage(unsigned char messageCode, String message);
void changeState(unsigned char newState);
void bootUpRoutine();



void setup()
{
    // nothing to do here everything is done in the Statemachine loop

    // setup serial for debugg messages
    #ifdef _DEBUGING
      Serial.begin(9600);
      Serial.println("Debug Messages activated:");
    #endif
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

void changeState(unsigned char newState)
{
  _previousState = _state;
  _state = newState;
  SEND_DEBUG_MESSAGE(0, "Changed State");
}

void bootUpRoutine()
{
  _display.begin();
  // Load Settings from EEPROM
  // Init Display
  // Init SD-Card
  // Load
  // Load infos from SD card





  // When all done:
  changeState(_STATE_HOME);
}
