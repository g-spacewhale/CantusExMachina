/* ------------------------------------------------- */
//
//      BandController - Class to manage MIDI Files
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */


/*
//  TODOS:
//          - SMPTE Time format implementation
//          -
*/


#include "MidiFileHandler.h"


MidiFileHandler::MidiFileHandler()
{
  // MIDI infos
  _format       = 0;
  _ntrks        = 0;
  _tpqn         = 0;
  _usPerMidiQn  = 500000;  // standard tempo = 120bpm -> 500000us
  _trackLen     = 0;
  _totalTime    = 0;

  // Helper vars
  _isPlaying             = false;
  _counter               = 0;
  _midiFile              = 0;
  _currentByte           = 0;
  _nextByte              = 0;
  _mightyStatusNibble    = 0;
  _punyStatusNibble      = 0;
  _channelPrefix         = 0;
}

void MidiFileHandler::setMidiFile(File *midiFile)
{
  // MIDI infos
  _counter = 0;
  _midiFile = midiFile;
}

String MidiFileHandler::getTitle() {return _title;}
uint16_t MidiFileHandler::getFormat() {return _format;}
uint16_t MidiFileHandler::getNtrks() {return _ntrks;}
uint16_t MidiFileHandler::getTpqn() {return _tpqn;}
uint32_t MidiFileHandler::getUsPerMidiQn() {return _usPerMidiQn;}
uint32_t MidiFileHandler::getTrackLen() {return _trackLen;}
uint32_t MidiFileHandler::getTotalTime() {return _totalTime;}
void MidiFileHandler::setPlaying(boolean playing){_isPlaying = playing;}

int8_t MidiFileHandler::getHeaderInfo()
{
	//initialize next byte

	_nextByte = _midiFile->read();

	// Header Title (check header title has to be MThd)
	if(!(getNextByte() == 'M' && getNextByte() == 'T' && getNextByte() == 'h' && getNextByte() == 'd'))
		return _MIDI_FILE_HANDLER_ERROR_HEADER_TITLE;

	// Header Length (check header length. has to be 6)
	if(getNextNBytesAsInt(4) != 6)
		return _MIDI_FILE_HANDLER_ERROR_HEADER_LENGTH;

	// MIDI Format (check midi format; has to be 0, 1 or 2)
  _format = getNextNBytesAsInt(2);
	if (_format != 0 && _format != 1 && _format != 2)
		return _MIDI_FILE_HANDLER_ERROR_HEADER_FORMAT;

	// Number of Tracks (check number of tracks. has to be 1 for format 0)
  _ntrks = getNextNBytesAsInt(2);
	if(_ntrks != 1 && _format == 0)
		return _MIDI_FILE_HANDLER_ERROR_HEADER_TRACKNUM;

	// Time Divisions (check division: information can be metrical or time-code-based.)
	if(!isStatus(_nextByte))
  {
		_tpqn = getNextNBytesAsInt(2);
  } else {  //add smpte time code check here
    return _MIDI_FILE_HANDLER_ERROR_HEADER_SMPTE;
	}
	return 0;
}

int8_t MidiFileHandler::getTrackInfo()
{
  startSong();

  _isPlaying = false; // don't play notes just parse

  while(_counter < _trackLen)
	{
    playSong();
  }
  return 0;
}

int8_t MidiFileHandler::startSong()
{
  _trackLen = 0;
	//check for correct track header, has to be MTrk
	if(!(getNextByte()  == 'M' && getNextByte()  == 'T' && getNextByte()  == 'r' && getNextByte()  == 'k'))
    return _MIDI_FILE_HADLER_ERROR_TRACK_TITLE;

	_trackLen = getNextNBytesAsInt(4);
}

int8_t MidiFileHandler::playSong()
{
  if(_isPlaying && _counter >= _trackLen)
    return _MIDI_END_OF_TRACK;

  if(_isPlaying)
  {
    delay(getDeltaTimeAsMillis());
  } else {
    getDeltaTimeAsMillis(); // just to get rid of the bytes
  }
  return getNextEvent();
}

int8_t MidiFileHandler::getNextEvent()
{
  //if next byte is status, save into status nibbles and get new byte, else don't
  //this is done, because the next byte somehow has to be read in. it eliminates confusion,
  //what byte is the currently 'active' one.
  if(isStatus(_nextByte))
  {
    _currentStatusByte = getNextByte();
    _mightyStatusNibble = getMightyNibbleOf(_currentStatusByte);
    _punyStatusNibble = getPunyNibbleOf(_currentStatusByte);
  } //else, the next byte is chained to the previous status byte, for it is not a status itself. (running Status)

  switch(_mightyStatusNibble)
  {
  //channel voice messages
    case _MIDI_MESSAGE_NOTE_OFF:
      // 1000	nnnn - note off event
      // nnnn - MIDI channel (puny_nibble)
      // 0kkkkkkk - the note
      // 0vvvvvvv - the velocity
      doNoteOff();
      break;

    case _MIDI_MESSAGE_NOTE_ON:
      // 1001	nnnn - note on event
      // nnnn - MIDI channel (puny_nibble)
      // 0kkkkkkk - the note
      // 0vvvvvvv - the velocity
      doNoteOn();
      break;

    case _MIDI_MESSAGE_KEY_AFTERTOUCH:
      // 1010	nnnn - polyphonic key pressure (aftertouch)
      // nnnn - MIDI channel (puny_nibble)
      // 0kkkkkkk - the note
      // 0vvvvvvv - the velocity
      doKeyAftertouch(_punyStatusNibble, getNextNBytesAsInt(1,true), getNextNBytesAsInt(1,true));
      break;

    case _MIDI_MESSAGE_CONTROL_CHANGE:
      // 1011	nnnn - control change
      // nnnn - MIDI channel (puny_nibble)
      // 0ccccccc - control number (0-119)
      //(120-127) are reserved for channel mode messages
      // 0vvvvvvv - controller value (0-127)
      doControlChange(_punyStatusNibble, getNextNBytesAsInt(1,true), getNextNBytesAsInt(1,true));
      break;

    case _MIDI_MESSAGE_PROGRAM_CHANGE:
      // 1100	nnnn - program change
      // nnnn - MIDI channel (puny_nibble)
      // 0ppppppp - new program number
      doProgramChange(_punyStatusNibble,getNextNBytesAsInt(1,true));
      break;

    case _MIDI_MESSAGE_CHANNEL_AFTERTOUCH:
      // 1101	nnnn - channel pressure (after touch)
      // nnnn - MIDI channel (puny_nibble)
      // 0vvvvvvv - greates pressure value of all current depressed keys
      doChannelAftertouch(_punyStatusNibble,getNextNBytesAsInt(1,true));
      break;

    case _MIDI_MESSAGE_PITCH_BEND:
      // 1110	nnnn - pitch bend change as 14 bits. 2000H is centre
      // 0lllllll - least significant bits of pitch bend change
      // 0mmmmmmm - most significant bits of pitch bend change
      doPitchBend(_punyStatusNibble,getNextNBytesAsInt(2,true));
      break;

    case _MIDI_MESSAGE_SYSTEM_MESSAGE:
      //system common messages
      return getSystemMessage(_punyStatusNibble);

    default:
      Serial.println(_currentByte);
      Serial.println("Status byte not found, termination.");
      return -1;
  }
  return 0;
}

int8_t MidiFileHandler::getSystemMessage(byte systemMessageCode)
{
  switch(systemMessageCode)
  {
    case _MIDI_SYSTEM_MESSAGE_EXCLUSIVE:                    // allows for own messages and allows additional midi messages.
      while(getNextByte() != 0xF7);                         //wait for end of exclusive byte
      break;

    // SYSTEM COMMON MESSAGES
    case _MIDI_SYSTEM_MESSAGE_COMMON_TIME_CODE:             //MIDI time code quarter frame | 0 nnn dddd - n = message type, d = values | these still need some alterations
    case _MIDI_SYSTEM_MESSAGE_COMMON_UNDEFINED_1:
    case _MIDI_SYSTEM_MESSAGE_COMMON_UNDEFINED_2:
    case _MIDI_SYSTEM_MESSAGE_COMMON_TUNE_REQUEST:          //tune request
      break;

    case _MIDI_SYSTEM_MESSAGE_COMMON_SONG_POSITION:         //song position pointer - 14 bit number that is midi beats | (one beat is six midi clocks) | 0lllllll - lsb
      getNextNBytesAsInt(2,true);
      break;

    case _MIDI_SYSTEM_MESSAGE_COMMON_SONG_SELECT:           //song select | 0sssssss - specifies sequence or song
      getNextNBytesAsInt(1,true);
      break;

    case _MIDI_SYSTEM_MESSAGE_COMMON_EOX:                   //end of exclusive, should always be caught by system exclusive while consumer.
      Serial.println("unexpected end of exclusive byte. terminating");
      return -1;

    //SYSTEM REAL-TIME MESSAGES (IGNORE FOR FILES)
    case _MIDI_SYSTEM_MESSAGE_RT_TIMING_CLOCK:  //timing clock. sent 24 times per quarter note when sync is required
    case _MIDI_SYSTEM_MESSAGE_RT_UNDEFINED_1:
    case _MIDI_SYSTEM_MESSAGE_RT_UNDEFINED_2:
    case _MIDI_SYSTEM_MESSAGE_RT_START:         //start. start the current sequence playing  (followed with timing clocks)
    case _MIDI_SYSTEM_MESSAGE_RT_CONTINUE:      //continue. continue at the point where stopped.
    case _MIDI_SYSTEM_MESSAGE_RT_STOP:          //stop - stop current sequence
    case _MIDI_SYSTEM_MESSAGE_RT_ACTIVE_SENSING:
      //active sensing. still alive ping for reciever.
      //use is optional. reciever should expect one every 300ms
      //after the first one. if not, connection terminated and turned off.
      break;

    // FILE META EVENTS
    case _MIDI_FILE_META_EVENT:
      return getMetaEvent(getNextByte());

   default:
     Serial.println(_currentByte);
     Serial.println("System exclusive not found, termination.");
     return -1;
  }
}

int8_t MidiFileHandler::getMetaEvent(byte metaEventCode)
{
  switch(metaEventCode)
  {
    case _MIDIE_FILE_META_EVENT_SEQUENCE_NUMBER:
      setSequenceNumber();
      break;

    case _MIDIE_FILE_META_EVENT_TEXT_EVENT:
      setTextEvent();
      break;

    case _MIDIE_FILE_META_EVENT_COPYRIGHT_NOTICE:
      setCopyrightNotice();
      break;

    case _MIDIE_FILE_META_EVENT_SEQUENCE_NAME:
      setSequenceName();
      break;

    case _MIDIE_FILE_META_EVENT_INSTRUMENT_NAME:
      setInstrumentName();
      break;

    case _MIDIE_FILE_META_EVENT_LYRIC:
      setLyric();
      break;

    case _MIDIE_FILE_META_EVENT_MARKER:
      setEventMarker();
      break;

    case _MIDIE_FILE_META_EVENT_CUE_POINT:
      setCuePoint();
      break;

    case _MIDIE_FILE_META_EVENT_MIDI_CHANNEL_PREFIX:
      setChannelPrefix();
      break;

    case _MIDIE_FILE_META_EVENT_END_OF_TRACK:
      return doEndOfTrack();

    case _MIDIE_FILE_META_EVENT_SET_TEMPO:
      setTempo();
      break;

    case _MIDIE_FILE_META_EVENT_SMPTE_OFFSET:
      setSMPTEOffset();
      break;

    case _MIDIE_FILE_META_EVENT_TIME_SIGNATURE: //this is important for time conversion.
      setTimeSignature();
      break;

    case _MIDIE_FILE_META_EVENT_KEY_SIGNATURE:
      setKeySignature();
      break;

    case _MIDIE_FILE_META_EVENT_SEQUENCER_SPECIFIC:
      doSequencerSpecificEvent();
      break;

    default:
      Serial.println(_currentByte);
      Serial.println("Meta event not found, termination.");
      return -1;
  }
  return 0;
}


void MidiFileHandler::doNoteOff()
{
  //uint8_t channel = _punyStatusNibble;
  //int8_t note = getNextNBytesAsInt(1,true);
  //int8_t velocity = getNextNBytesAsInt(1,true);

  byte note = getNextByte();
  byte velocity = getNextByte();

  if(_isPlaying)
  {
    //Serial.println("Note Off ("+String(_currentStatusByte, HEX)+") [Note: "+String(note, HEX)+ " | Velocity: "+String(velocity, HEX)+"]");

    Serial1.write(_currentStatusByte);
    Serial1.write(note);
    Serial1.write(velocity);
  }
}

void MidiFileHandler::doNoteOn()
{
  //uint8_t channel = _punyStatusNibble;
  //int8_t note = getNextNBytesAsInt(1,true);
  //int8_t velocity = getNextNBytesAsInt(1,true);

  byte note = getNextByte();
  byte velocity = getNextByte();

  if(_isPlaying)
  {
    //Serial.println("Note On ("+String(_currentStatusByte, HEX)+") [Note: "+String(note, HEX)+ " | Velocity: "+String(velocity, HEX)+"]");

    Serial1.write(_currentStatusByte);
    Serial1.write(note);
    Serial1.write(velocity);
  }
}

void MidiFileHandler::doKeyAftertouch(uint8_t channel, uint8_t note, uint16_t pressure)
{
  //Serial.println("Aftertouch (#"+String(note, DEC)+") on [Channel: "+String(channel, DEC)+ " | Pressure: "+String(velocity, DEC)+"]");
  if(_isPlaying)
  {
    Serial1.write(_currentStatusByte);
    Serial1.write(note);
    Serial1.write(pressure);
  }
}

void MidiFileHandler::doControlChange(uint8_t channel, uint16_t controllerNumber, uint16_t controllerValue)
{
	if(controllerNumber <= 119)
	{
		//do send command
		return;
	}
	else
	{
		switch (controllerNumber)
		{
			//all sounds off event
			case 120:
			break;

			//reset all controllers event
			case 121:
			//controller value should always be zero, reffer to manual for further information
			break;

			//local control event
			//controller value 127 is on
			// controller value 0 is off
			case 122:
			break;

			//all notes off event
			case 123:
			break;

			//omni mode off event
			//and all notes off event
			case 124:
			break;

			//omni mode on event
			//and all notes off event
			case 125:
			break;

			//mono mode on (poly off) event
			//and all notes off event
			case 126:
			break;

			//mono mode off (poly on) event
			//and all notes off event
			case 127:
			break;
		}
	}
}


void MidiFileHandler::doProgramChange(uint8_t channel, uint8_t programNumber)
{
  if(_isPlaying)
  {

  } else {

  }
}

void MidiFileHandler::doChannelAftertouch(uint8_t channel, uint16_t pressure)
{
  //Serial.println("Channel aftertouch on [Channel: "+String(channel, DEC)+ " | Pressure: "+String(pressure, DEC)+"]");
  if(_isPlaying)
  {
    Serial1.write(_currentStatusByte);
    Serial1.write(pressure);
  }
}

void MidiFileHandler::doPitchBend(uint8_t channel, uint32_t pitchBendChange)
{
  //Serial.println("Pitch Bend on [Channel: "+String(channel, DEC)+ " | Pitch Bend Change: "+String(pitchBendChange, DEC)+"]");
  if(_isPlaying)
  {
  }
}

void MidiFileHandler::setSequenceNumber()
{
  //FF 00 02 ssss - sequence number
  //at beginning of track before nonzero delta times
  //shows sequence number
  //ssss 1 - 6 bit value

  int32_t sequencerNumber;
  getNextByte(); //this seems to be a 02 always = len
  sequencerNumber = getNextNBytesAsInt(2);

  sequencerNumber = sequencerNumber;  // just so no warning appear

}

void MidiFileHandler::setTextEvent()
{
  //FF 01 len text - text event
  //Serial.println("new Text Event!");

  uint32_t length;
  String text = "";
  byte temp;

  length = getVariableLength();
  for(uint32_t i = 0; i < length; i++)
  {
    //text = text + getNextByte();
    temp = getNextByte();
    text = text + char(temp);
  }
}

void MidiFileHandler::setCopyrightNotice()
{
  //FF 02 len text - copyright notice
  //Serial.println("new Copyright notice!");

  uint32_t length;
  String text = "";
  byte temp;

  length = getVariableLength();
  for(uint32_t i = 0; i < length; i++)
  {
    //text = text + getNextByte();
    temp = getNextByte();
    text = text + char(temp);
  }
}

void MidiFileHandler::setSequenceName()
{
  //FF 03 len text - sequence track name
  //Serial.println("new Sequence/Track name!");
  uint32_t length;
  String text = "";
  byte temp;

  length = getVariableLength();
  for(uint32_t i = 0; i < length; i++)
  {
    //text = text + getNextByte();
    temp = getNextByte();
    text = text + char(temp);
  }
  _title = text;
}

void MidiFileHandler::setInstrumentName()
{
  //FF 04 len text - Instrument Name
  //Serial.println("new Instrument Description");

  uint32_t length;
  String text = "";
  byte temp;

  length = getVariableLength();
  for(uint32_t i = 0; i < length; i++)
  {
    //text = text + getNextByte();
    temp = getNextByte();
    text = text + char(temp);
  }
}

void MidiFileHandler::setLyric()
{
  //FF 05 len text - Lyric
  //Serial.println("new Lyric");

  uint32_t length;
  String text = "";
  byte temp;

  length = getVariableLength();
  for(uint32_t i = 0; i < length; i++)
  {
    //text = text + getNextByte();
    temp = getNextByte();
    text = text + char(temp);
  }
}

void MidiFileHandler::setEventMarker()
{
  //FF 06 len text - Marker
  //Serial.println("new Marker!");

  uint32_t length;
  String text = "";
  byte temp;

  length = getVariableLength();
  for(uint32_t i = 0; i < length; i++)
  {
    //text = text + getNextByte();
    temp = getNextByte();
    text = text + char(temp);
  }
}

void MidiFileHandler::setCuePoint()
{
  //FF 07 len text - cue point
  //Serial.println("new Cue point!");

  uint32_t length;
  String text = "";
  byte temp;

  length = getVariableLength();
  for(uint32_t i = 0; i < length; i++)
  {
    //text = text + getNextByte();
    temp = getNextByte();
    text = text + char(temp);
  }
}

void MidiFileHandler::setChannelPrefix()
{
  //FF 20 02 cc  - MIDI channel prefix
  getNextByte(); // next one is always 01
  _channelPrefix = getNextByte();
}

int8_t MidiFileHandler::doEndOfTrack()
{
  //FF 2F 00 - End of Track
  //getNextByte(); // next one is always 00 but next byte would be EOF
  Serial.println("end of track reached");
  return _MIDI_END_OF_TRACK;
}

void MidiFileHandler::setTempo()
{
  //FF 51 03  - Set Tempo
  //Serial.println("changed tempo to "+String(_us_per_midi_qn,DEC));

  getNextByte(); // next one is always 01
  _usPerMidiQn = getNextNBytesAsInt(3);
}

void MidiFileHandler::setSMPTEOffset()
{
  //FF 54 05 hr mn se fr ff - SMPTE Offset

  int16_t hr, mn, se, fr, ff = 0;

  getNextByte(); // next one is always 05
  hr = getNextByte();
  mn = getNextByte();
  se = getNextByte();
  fr = getNextByte();
  ff = getNextByte();

  hr = hr;
  mn = mn;
  se = se;
  fr = fr;
  ff = ff;
}

void MidiFileHandler::setTimeSignature()
{
  //FF 58 04 nn dd cc bb - Time signature
  //this is important for time conversion.

  int16_t numerator, denominator, clocks_per_click, notated_32nd_per_qn = 0;

  getNextByte(); // next one is always 04
  numerator = getNextByte();
  denominator = getNextByte();
  clocks_per_click = getNextByte();
  notated_32nd_per_qn = getNextByte();

  numerator = numerator;
  denominator = denominator;
  clocks_per_click = clocks_per_click;
  notated_32nd_per_qn = notated_32nd_per_qn;

}

void MidiFileHandler::setKeySignature()
{
  byte sf, mi = 0;
  //FF 59 02 sf mi - key signature
  getNextByte(); // next one is always 2
  sf = getNextByte();
  mi = getNextByte();

  sf = sf;
  mi = mi;
}

void MidiFileHandler::doSequencerSpecificEvent()
{
  //FF 7F len data - sequencer specific meta event
  //Serial.println("new sequencer specific meta event!");

  uint32_t length;
  String text = "";
  byte temp;

  length = getVariableLength();
  for(uint32_t i = 0; i < length; i++)
  {
    //text = text + getNextByte();
    temp = getNextByte();
    text = text + char(temp);
  }
}




// Helper functions
byte MidiFileHandler::getNextByte()
{
  _counter++;
	_currentByte = _nextByte;
	_nextByte = _midiFile->read();
	return _currentByte;
}

boolean MidiFileHandler::isStatus(byte nextByte)
{
	if(nextByte & 0x80)//1000 0000
		return 1;
	else
		return 0;
}

int32_t MidiFileHandler::getDeltaTimeAsMillis()
{
  int32_t deltaTime = getVariableLength() * (_usPerMidiQn / _tpqn) / 1000;
  _totalTime += deltaTime;
  return deltaTime;
}

int32_t MidiFileHandler::getVariableLength()
{
  int32_t number = 0;
  byte helperByte = 0;
  do
  {
    helperByte = getNextByte();
    helperByte = helperByte & 0x7F;
    number = number << 7 | helperByte;
  } while(isStatus(_currentByte)); //check if this was the last one (not of format 1xxxxxxx)
  return number;
}

//converts up to 4 bytes into an int. most significant first.
//different to next n data bytes to int, because these bytes are pure int and
//do not have the 0xxxxxxx data byte indication that has to be bitshifted away
int32_t MidiFileHandler::getNextNBytesAsInt(int8_t numberOfBytes, boolean isDataByte)
{
  int32_t number = 0;

	if(numberOfBytes <= 4)
	{
		for(int i=0; i < numberOfBytes; i++)
					number = number << (isDataByte ? 7 : 8 ) | getNextByte();
		return number;
	}	else {
		Serial.println("to many bytes to form a long int where asked for! termination");
    return -1;
  }
}

byte MidiFileHandler::getPunyNibbleOf(byte thisByte) {return thisByte & 0x0F;} // ANDs the byte with 0000 1111 to eliminate mighty nibble
byte MidiFileHandler::getMightyNibbleOf(byte thisByte) {return thisByte >> 4;} //moves all bits to the right four steps to create mighty nibble
