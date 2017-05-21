/* ------------------------------------------------- */
//
//         BandController - Definitions for Errorcodes
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#ifndef ErrorCodes_h
#define ErrorCodes_h

#include <Arduino.h>

#define _ERROR_NOERROR 0
#define _ERROR_SDCARD 1
#define _ERROR_MIDI_DIR_NOT_FOUND 2
#define _ERROR_CONVERT 3

String _errorMessages[] = {
  "No Error",
  "SD-Card Error",
  "MDID Directory Dot Found",
  "Converting not Possible"
};

#endif
