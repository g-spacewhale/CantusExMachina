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
#include <SD.h>

// custom classes and additional headers for information
#include <Defines.h>        // holds all Defines
#include <ErrorCodes.h>     // Definitions of ErrorCodes
#include <DataManagement.h> // custom class to manage data on sd card
#include <Language.h>       //
#include <Display.h>        // custom Display class to manage the tft Display

// ------------------- Variable declarations -------------------
// Settings
unsigned char _language = _languageEnglish;
unsigned char _displayBrightness = 255;

volatile char _encoderCounterTurns = 0;
volatile unsigned char _encoderCounterClicks = 0;

uint16_t _errorCode = 0;
uint16_t _currPosition = 0;

unsigned char _state = _STATE_BOOTUP;       // holds the current state of the statemachine - refere to 'Defining of States'
unsigned char _previousState = 0;           // holds the previous state of the statemachine

Display _display = Display(_displayPinCS, _displayPinDC, _displayPinRST, _displayPinBACKLIGHT, _displayBrightness, _sdPinCS, _languageEnglish);
DataManagement _dataManager = DataManagement(_sdPinCS);
Language _translation = Language();

// ------------------- Forward declarations --------------------
void sendDebugMessage(unsigned char messageCode, String message);
void error(unsigned char errorCode);

void encoderInterruptTurn();
void encoderInterruptClick();
char encoderGetTurns();
unsigned char encoderGetClicks();
void encoderResetTurns();
void encoderResetClicks();

void changeState(unsigned char newState);
void initState(unsigned char newState);
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

    changeState(_STATE_BOOTUP);
}

// Statemachine Loop
void loop()
{
  switch (_state)
  {
    case _STATE_BOOTUP:
    case _STATE_ERROR:
      // all these states don't need a loop
      break;

    case _STATE_HOME:
      // check if encoder was rotated
      if(encoderGetTurns())
      {
        // change position
        _currPosition += encoderGetTurns();
        _currPosition = _currPosition % 2; // transform to binary
        encoderResetTurns();
        _display.changeHomeScreenSelection(_currPosition);
      }

      // check if encoder was clicked
      if(encoderGetClicks())
      {
        encoderResetClicks();
        if(_currPosition == 0) // Songs was selected
        {
          //changeState(_STATE_SONG_SELECTION);
          error(_ERROR_NOERROR);
        } else {  // Settings was selected
          //changeState(_STATE_SETTINGS);
          error(_ERROR_NOERROR);
        }
      }
      break;

    case _STATE_SONG_SELECTION:

      break;

    case _STATE_SETTINGS:

      break;

    default:
      //Error message
      SEND_DEBUG_MESSAGE(0, "ERROR: Statemachine went into default case");
      break;
  }
}

void changeState(unsigned char newState)
{
  _previousState = _state;
  _state = newState;
  SEND_DEBUG_MESSAGE(0, "Changed State");
  initState(_state);
}

// intitialization of each state after changing the state
void initState(unsigned char newState)
{
  switch (_state)
  {
    case _STATE_BOOTUP:
      bootUpRoutine();
      break;

    case _STATE_ERROR:
      _display.displayErrorMessage(_errorCode, _errorMessages[_errorCode]);
      break;

    case _STATE_HOME:
      // init Home (draw menu for first time)
      _display.displayHomeScreen();
      break;

    case _STATE_SONG_SELECTION:

      break;

    case _STATE_SETTINGS:

      break;

    default:
      //Error message
      SEND_DEBUG_MESSAGE(0, "ERROR: initState went into default case (newState = "+String(newState)+")");
      break;
  }
}

void bootUpRoutine()
{
  char sdError = 0;

  // Load Settings from EEPROM
  _language = EEPROM.read(_EEPROM_LANGUAGE);
  SEND_DEBUG_MESSAGE(0, "Language from EEPROM = "+String(_language, DEC));
  if(_language !=_languageEnglish || _language != _languageGerman)
  {
    _language = _languageEnglish;
    EEPROM.write(_EEPROM_LANGUAGE, _language);
  }

  _displayBrightness = EEPROM.read(_EEPROM_DISPLAY_BRIGHTNESS);
  SEND_DEBUG_MESSAGE(0, "Display Brightness from EEPROM = "+String(_displayBrightness, DEC));
  if(_displayBrightness < 0 || _displayBrightness > 255)
  {
    _displayBrightness = 255;
    EEPROM.write(_EEPROM_DISPLAY_BRIGHTNESS, _displayBrightness);
  }

  sdError = !_dataManager.begin();

  // Init Display
  _display.changeLanguage(_language);
  _display.setBacklight(_displayBrightness);
  _display.begin();
  _display.displayBootupScreen();


  // Init SD-Card
  _display.changeBootupInfo("Initializing SD-Card");
  if(sdError)
  {
    error(_ERROR_SDCARD);
    return;
  }

  _display.changeBootupInfo("Load languageFile");

  // Load infos from SD card


  _display.changeBootupInfo("Parse Song Info");

  //delay(5000);

  _display.changeBootupInfo("bootUpRoutine done");

  // When all done:
  changeState(_STATE_HOME);
}

void sendDebugMessage(unsigned char messageCode, String message)
{
  Serial.print("DebuggMessage(#");
  Serial.print(messageCode);
  Serial.print("):");
  Serial.println(message);
}


void error(unsigned char errorCode)
{
  _errorCode = errorCode;
  SEND_DEBUG_MESSAGE(100+errorCode, _errorMessages[_errorCode]);
  changeState(_STATE_ERROR);
}

void encoderInterruptTurn()
{
  if (digitalRead(_encoderPinA) == HIGH)    // found a low-to-high on channel A
  {
    if (digitalRead(_encoderPinB) == LOW)   // check channel B to see which way
      _encoderCounterTurns++;                // encoder is turning: CCW
    else
      _encoderCounterTurns--;                // encoder is turning:CW
  } else {                                  // found a high-to-low on channel A
    if (digitalRead(_encoderPinB) == LOW)   // check channel B to see which way
      _encoderCounterTurns--;                // encoder is turning:CW
    else
      _encoderCounterTurns++;                // encoder is turning:CCW
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
