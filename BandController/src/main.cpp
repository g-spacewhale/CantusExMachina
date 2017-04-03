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

// custom classes and additional headers for information
#include <StateDefines.h>
#include <Display.h>

//#define _DEBUGGING 1   // defines if debugg messages are sent via serial

#define _FIRMWARE_VERSION 0.01

// EEPROM Cell codes
#define _EEPROM_DISPLAY_BRIGHTNESS 0
#define _EEPROM_LANGUAGE 1

// ------------------ Defining of Pin mapping ------------------
// defines the 3 Data-Pins of the Roatary Encoder
#define _encoderPinA 2
#define _encoderPinB 3
#define _endocerPinClick 18

/*#define _displayPinCS 53
#define _displayPinDC 9
#define _displayPinRST 8
#define _displayPinBACKLIGHT 4*/



// ------------------- Variable declarations -------------------
unsigned char _state = _STATE_BOOTUP;       // holds the current state of the statemachine - refere to 'Defining of States'
//unsigned char _previousState = 0;           // holds the previous state of the statemachine

Display _display = Display(35, 9, 8, 4);


// forward declarations
//void sendDebugMessage(unsigned char messageCode, String message);
//void changeState(unsigned char newState);
void bootUpRoutine();


// Start of Code
void setup()
{
    // nothing to do here everything is done in the Statemachine loop

    // setup serial for debugg messages

}

// Statemachine Loop
void loop()
{
  switch (_state)
  {
    case _STATE_BOOTUP:
      bootUpRoutine();
      _state = _STATE_HOME;
      break;
    case _STATE_HOME:
      break;
    default:
      //Error message
      break;
  }
}

/*void changeState(unsigned char newState)
{
  _previousState = _state;
  _state = newState;
}*/

void bootUpRoutine()
{
  _display.begin();
  // Load Settings from EEPROM
  // Init Display
  // Init SD-Card
  // Load
  // Load infos from SD card
}
