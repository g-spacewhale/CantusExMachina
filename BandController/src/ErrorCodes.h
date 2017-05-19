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

String _errorMessages[] = {
  "No Error",
  "SD-Card Error"
};

#endif
