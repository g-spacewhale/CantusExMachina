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
#include <MidiFileHandler.h>
#include <Language.h>       //
#include <Display.h>        // custom Display class to manage the tft Display

// ------------------- Variable declarations -------------------
// Settings
unsigned char _language = _languageEnglish;
unsigned char _displayBrightness = 0;

volatile char _encoderCounterTurns = 0;
volatile unsigned char _encoderCounterClicks = 0;

int32_t _time = 0;

uint16_t _errorCode = _ERROR_NOERROR;
int16_t _currPosition = 0;
int16_t _currValue = 0;
int16_t _selection = 0;
boolean _changingValue = false;

unsigned char _state = _STATE_BOOTUP;       // holds the current state of the statemachine - refere to 'Defining of States'
unsigned char _previousState = 0;           // holds the previous state of the statemachine

DataManagement _dataManager = DataManagement(_sdPinCS);
Display _display = Display(_displayPinCS, _displayPinDC, _displayPinRST, _displayPinBACKLIGHT, _displayBrightness, _languageEnglish, &_dataManager);
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
void changeLanguage(unsigned char newLanguage);
void changeBrightness(unsigned char newBrightness);
void initState(unsigned char newState);
void bootUpRoutine();

void setup()
{
    Serial.begin(9600);
    while (!Serial);
    Serial1.begin(31250);   // MIDI Port
    while (!Serial1);

    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("----------------------- BandController Start -----------------------");
    Serial.println("");

    // setup serial for debugg messages
    #ifdef _DEBUGING
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
          changeState(_STATE_SONG_SELECTION);
        } else {  // Settings was selected
          changeState(_STATE_SETTINGS);
        }
      }
      break;

    case _STATE_SONG_SELECTION:
      if(encoderGetTurns())
      {
        // change position
        _currPosition += encoderGetTurns();
        _currPosition = _currPosition % (_dataManager.getSongCount()+1);
        if(_currPosition < 0) _currPosition = (_dataManager.getSongCount()+1) + _currPosition;
        encoderResetTurns();
        _display.changeSongsSelection(_currPosition);
      }

      // check if encoder was clicked
      if(encoderGetClicks())
      {
        encoderResetClicks();
        if(_currPosition == int(_dataManager.getSongCount())) // Return was selected
        {
          changeState(_STATE_HOME);
        } else {  // A song was selecte
          // play or convert song
          Song temp = _dataManager.getSong(_currPosition);

          if(temp.getFormat() == 0)
          {
            _selection = _currPosition;
            changeState(_STATE_PLAY_SONG);
          } else {
            // convert song
            // error(_ERROR_CONVERT);
          }
        }
      }
      break;

    case _STATE_SETTINGS:

      if(!_changingValue)
      {
        // check if encoder was rotated
        if(encoderGetTurns())
        {
          // change position
          _currPosition += encoderGetTurns();
          _currPosition = _currPosition % _SETTINGS_COUNT; // transform to max selections
          if(_currPosition < 0) _currPosition = _SETTINGS_COUNT + _currPosition;
          encoderResetTurns();
          _display.changeSettingsSelection(_currPosition);
        }

        // check if encoder was clicked
        if(encoderGetClicks())
        {
          encoderResetClicks();
          if(_currPosition == _SETTINGS_RETURN)
          {
            changeState(_STATE_HOME);
          } else {
            _changingValue = true;

            switch (_currPosition)
            {
              case _SETTINGS_LANGUAGE:
                _currValue = _language;
                break;
              case _SETTINGS_BRIGHTNESS:
                _currValue = _displayBrightness;
            }

            _display.changeSettingsValue(_currPosition, _currValue);
          }
        }
      } else {
        // changing Vallue
        if(encoderGetTurns())
        {
          // change position
          switch (_currPosition)
          {
            case _SETTINGS_LANGUAGE:
              _currValue += encoderGetTurns();
              _currValue = _currValue % _LANGUAGE_COUNT;
              if(_currValue < 0) _currValue = _LANGUAGE_COUNT + _currValue;
              break;
            case _SETTINGS_BRIGHTNESS:
              _currValue += encoderGetTurns()*5;
              if(_currValue < 5) _currValue = 5;
              if(_currValue > 100) _currValue = 100;
          }

          encoderResetTurns();
          _display.changeSettingsValue(_currPosition, _currValue);
        }

        // check if encoder was clicked
        if(encoderGetClicks())
        {
          encoderResetClicks();
          _changingValue = false;

          switch (_currPosition)
          {
            case _SETTINGS_LANGUAGE:
              changeLanguage(_currValue);
              _display.displaySettings();
              break;
            case _SETTINGS_BRIGHTNESS:
              changeBrightness(_currValue);
              break;
          }
          _display.changeSettingsSelection(_currPosition);
        }
      }
      break;

    case _STATE_PLAY_SONG:

      if(_dataManager.playSong() == _MIDI_END_OF_TRACK) // at end of track
      {
        Serial.println("Needed time: "+String((millis()-_time) ,DEC));
        Serial.println("Actual time: "+String(_dataManager.getSong(_selection).getLength(),DEC));

        changeState(_STATE_SONG_SELECTION);
      } else {
        _display.changePlaySongTime(_selection);
      }

      // check if encoder was clicked
      if(encoderGetClicks())
      {
        encoderResetClicks();
        // Stop was selected
        _dataManager.stopSong();
        changeState(_STATE_SONG_SELECTION);
      }

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
  _currPosition = 0;
  encoderResetTurns();
  encoderResetClicks();
  initState(_state);
  SEND_DEBUG_MESSAGE(0, "Changed State");
}

void changeLanguage(unsigned char newLanguage)
{
  _language = newLanguage;
  _display.changeLanguage(_language);
  EEPROM.write(_EEPROM_LANGUAGE, _language);
}

void changeBrightness(unsigned char newBrightness)
{
  _displayBrightness = newBrightness;
  _display.changeBrightness(_displayBrightness);
  EEPROM.write(_EEPROM_DISPLAY_BRIGHTNESS, _displayBrightness);
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
        _selection = 0;
      _display.displayHomeScreen();
      break;

    case _STATE_SONG_SELECTION:
      _currPosition = _selection;
      _display.displaySongs(_selection);
      break;

    case _STATE_SETTINGS:
      _display.displaySettings();
      break;

    case _STATE_PLAY_SONG:
      _dataManager.startSong(_selection);
      _display.displayPlaySong(_selection);
      _time = millis();
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
  if(_language !=_languageEnglish && _language != _languageGerman)
  {
    _language = _languageEnglish;
    EEPROM.write(_EEPROM_LANGUAGE, _language);
  }

  _displayBrightness = EEPROM.read(_EEPROM_DISPLAY_BRIGHTNESS);
  SEND_DEBUG_MESSAGE(0, "Display Brightness from EEPROM = "+String(_displayBrightness, DEC));
  if(_displayBrightness < 5)
  {
    _displayBrightness = 5;
    EEPROM.write(_EEPROM_DISPLAY_BRIGHTNESS, _displayBrightness);
  } else if(_displayBrightness > 100){
      _displayBrightness = 100;
      EEPROM.write(_EEPROM_DISPLAY_BRIGHTNESS, _displayBrightness);
  }

  sdError = !_dataManager.begin();

  // Init Display
  _display.changeLanguage(_language);
  _display.changeBrightness(_displayBrightness);
  _display.begin();
  _display.displayBootupScreen();

  // Init SD-Card
  _display.changeBootupInfo("Initializing SD-Card");
  if(sdError)
  {
    error(_ERROR_SDCARD);
    return;
  }

  // _display.changeBootupInfo("Load languageFile");

  // Load infos from SD card

  _display.changeBootupInfo("Parse Song Info");

  switch (_dataManager.parseSongInfos())
  {
    case _ERROR_MIDI_DIR_NOT_FOUND:
      error(_ERROR_MIDI_DIR_NOT_FOUND);
      break;
    default:  // everything okay
      break;
  }

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
