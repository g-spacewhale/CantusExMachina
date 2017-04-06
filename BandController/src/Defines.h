/* ------------------------------------------------- */
//
//         BandController - general Definitions
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#ifndef Defines_h
#define Defines_h

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

// Defining of the 'Names' for the States of the Statemachine
// State 0 reserved for init of previousState
#define _STATE_ERROR 0
#define _STATE_BOOTUP 1
#define _STATE_SD_ERROR 2
#define _STATE_HOME 3

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

#define _sdPinCS 48

#endif
