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
#define _STATE_HOME 2
#define _STATE_SONG_SELECTION 3
#define _STATE_SETTINGS 4
#define _STATE_PLAY_SONG 5

/*// ------------------ Defining of Pin mapping ------------------
// defines the Data-Pins for the midi connection
#define _midiPinTx 18 // has to be a Serial TX pin (Pin 18 -> Serial 1)

// defines the Data-Pins of the Roatary Encoder
#define _encoderPinA 2  // has to be an interuppt pin
#define _encoderPinB 3
#define _encoderPinClick 19 // has to be an interuppt pin

// defines the Data-Pins of the Display
#define _displayPinCS 53
#define _displayPinDC 9
#define _displayPinRST 8
#define _displayPinBACKLIGHT 7 // pwm pin
// not defined but also connected
// MISO Pin 50
// MOSI Pin 51
// SCK Pin 52

// defines the Data-Pins of the SD-Card-Reader
#define _sdPinCS 49*/

// defines the Data-Pins for the midi connection
#define _midiPinTx 18 // has to be a Serial TX pin (Pin 18 -> Serial 1)

// defines the Data-Pins of the Roatary Encoder
#define _encoderPinA 3  // has to be an interuppt pin
#define _encoderPinB 4
#define _encoderPinClick 2 // has to be an interuppt pin

// defines the Data-Pins of the Display & SD-Card Reader
#define _displayPinDC 41
#define _displayPinRST 43
#define _sdPinCS 47
#define _displayPinCS 49
// not defined but on SPI connected
// MOSI Pin 51
// MISO Pin 50
// SCK Pin 52
#define _displayPinBACKLIGHT 45 // pwm pin


#endif
