/* ------------------------------------------------- */
//
//      BandController - Class to manage MIDI Files
//         created by Lukas Kaiser (mr14b042)
//                     Apr 2017
//
/* ------------------------------------------------- */

#ifndef MidiFileHandler_h
#define MidiFileHandler_h

#include <Arduino.h>
#include <SD.h>
#include <MIDI.h>

#define _MIDI_FILE_HANDLER_ERROR_HEADER_TITLE -1
#define _MIDI_FILE_HANDLER_ERROR_HEADER_LENGTH -2
#define _MIDI_FILE_HANDLER_ERROR_HEADER_FORMAT -3
#define _MIDI_FILE_HANDLER_ERROR_HEADER_TRACKNUM -4
#define _MIDI_FILE_HANDLER_ERROR_HEADER_SMPTE -5
#define _MIDI_FILE_HADLER_ERROR_TRACK_TITLE -6
#define _MIDI_END_OF_TRACK -100

// Channel Voice Messages
#define _MIDI_MESSAGE_NOTE_OFF                      0x08    // Note Off
#define _MIDI_MESSAGE_NOTE_ON                       0x09    // Note On (a velocity of 0 = Note Off)
#define _MIDI_MESSAGE_KEY_AFTERTOUCH                0x0A    // Polyphonic key pressure/Aftertouch
#define _MIDI_MESSAGE_CONTROL_CHANGE                0x0B    // Control change
#define _MIDI_MESSAGE_PROGRAM_CHANGE                0x0C    // Program change
#define _MIDI_MESSAGE_CHANNEL_AFTERTOUCH            0x0D    // Channel pressure/Aftertouch
#define _MIDI_MESSAGE_PITCH_BEND                    0x0E    // Pitch bend change
// Channel Mode Messages
#define _MIDI_MESSAGE_SELECT_CHANEL_MODE            0x0B    // Select channel Mode
// System Messages
#define _MIDI_MESSAGE_SYSTEM_MESSAGE                0x0F    // System Exclusive / System Common / System Real Time

#define _MIDI_SYSTEM_MESSAGE_EXCLUSIVE              0x00    // System Exclusive Messages
#define _MIDI_SYSTEM_MESSAGE_COMMON_TIME_CODE       0x01    // MIDI Time Code Quarter Frame
#define _MIDI_SYSTEM_MESSAGE_COMMON_SONG_POSITION   0x02    // Song Position pointer
#define _MIDI_SYSTEM_MESSAGE_COMMON_SONG_SELECT     0x03    // Song Select
#define _MIDI_SYSTEM_MESSAGE_COMMON_UNDEFINED_1     0x04    // Undefined
#define _MIDI_SYSTEM_MESSAGE_COMMON_UNDEFINED_2     0x05    // Undefined
#define _MIDI_SYSTEM_MESSAGE_COMMON_TUNE_REQUEST    0x06    // Tune Request
#define _MIDI_SYSTEM_MESSAGE_COMMON_EOX             0x07    // EOX: "End of System Exclusive" flag

#define _MIDI_SYSTEM_MESSAGE_RT_TIMING_CLOCK        0x08    // Timing Clock
#define _MIDI_SYSTEM_MESSAGE_RT_UNDEFINED_1         0x09    // Undefined
#define _MIDI_SYSTEM_MESSAGE_RT_START               0x0A    // Start
#define _MIDI_SYSTEM_MESSAGE_RT_CONTINUE            0x0B    // Continue
#define _MIDI_SYSTEM_MESSAGE_RT_STOP                0x0C    // Stop
#define _MIDI_SYSTEM_MESSAGE_RT_UNDEFINED_2         0x0D    // Undefined
#define _MIDI_SYSTEM_MESSAGE_RT_ACTIVE_SENSING      0x0E    // Active Sensing
#define _MIDI_SYSTEM_MESSAGE_RT_SYSTEM_RESET        0x0F    // Sytem Reset

// Meta Events
#define _MIDI_FILE_META_EVENT                       0x0F    // File Meta Event

#define _MIDIE_FILE_META_EVENT_SEQUENCE_NUMBER      0x00    // Sequence number
#define _MIDIE_FILE_META_EVENT_TEXT_EVENT           0x01
#define _MIDIE_FILE_META_EVENT_COPYRIGHT_NOTICE     0x02
#define _MIDIE_FILE_META_EVENT_SEQUENCE_NAME        0x03
#define _MIDIE_FILE_META_EVENT_INSTRUMENT_NAME      0x04
#define _MIDIE_FILE_META_EVENT_LYRIC                0x05
#define _MIDIE_FILE_META_EVENT_MARKER               0x06
#define _MIDIE_FILE_META_EVENT_CUE_POINT            0x07
#define _MIDIE_FILE_META_EVENT_MIDI_CHANNEL_PREFIX  0x20
#define _MIDIE_FILE_META_EVENT_END_OF_TRACK         0x2F
#define _MIDIE_FILE_META_EVENT_SET_TEMPO            0x51
#define _MIDIE_FILE_META_EVENT_SMPTE_OFFSET         0x54
#define _MIDIE_FILE_META_EVENT_TIME_SIGNATURE       0x58
#define _MIDIE_FILE_META_EVENT_KEY_SIGNATURE        0x59
#define _MIDIE_FILE_META_EVENT_SEQUENCER_SPECIFIC   0x7F


class MidiFileHandler
{
  public:
    MidiFileHandler();

    void      setMidiFile(File *midiFile);

    // Getter
    String    getTitle();
    uint16_t  getFormat();
    uint16_t  getNtrks();
    uint16_t  getTpqn();
    uint32_t  getUsPerMidiQn();
    uint32_t  getTrackLen();
    uint32_t  getTotalTime();

    void setPlaying(boolean playing = true);

    int8_t    getHeaderInfo();
    int8_t    getTrackInfo();
    int8_t    startSong();
    int8_t    playSong();
    int8_t    stopSong();
    int8_t    getNextEvent();

  private:
    // MIDI infos
    String    _title;        // Sequence name
    uint16_t  _format;       // midi format
    uint16_t  _ntrks;        // number of tracks
    uint16_t  _tpqn;         // ticks per quoter note
    uint32_t  _usPerMidiQn;  // micro second per midi quoter note
    uint32_t  _trackLen;     // length of track in bytes for format 0 files
    uint32_t  _totalTime;    // total playtime for format 0 files

    // Helper vars
    boolean   _isPlaying;
    uint32_t  _counter;
    uint32_t  _timeSinceLast;
    File*     _midiFile;
    byte      _currentByte;
    byte      _nextByte;
    byte      _currentStatusByte;
    byte      _mightyStatusNibble;
    byte      _punyStatusNibble;
    byte      _channelPrefix;

    // MIDI functions
    int8_t getSystemMessage(byte systemMessageCode);
    int8_t getMetaEvent(byte metaEventCode);

    void doNoteOff();
    void doNoteOn();
    void doKeyAftertouch(uint8_t channel, uint8_t note, uint16_t velocity);
    void doAllNotesOff(uint8_t channel);
    void doControlChange(uint8_t channel, uint16_t controllerNumber, uint16_t controllerValue);
    void doProgramChange(uint8_t channel, uint8_t programNumber);
    void doChannelAftertouch(uint8_t channel, uint16_t pressure);
    void doPitchBend(uint8_t channel, uint32_t pitchBendChange);

    void setSequenceNumber();
    void setTextEvent();
    void setCopyrightNotice();
    void setSequenceName();
    void setInstrumentName();
    void setLyric();
    void setEventMarker();
    void setCuePoint();
    void setChannelPrefix();
    int8_t doEndOfTrack();
    void setTempo();
    void setSMPTEOffset();
    void setTimeSignature();
    void setKeySignature();
    void doSequencerSpecificEvent();

    // Helper functions
    byte      getNextByte();
    boolean   isStatus(byte nextByte);
    int32_t   getDeltaTimeAsMillis();
    int32_t   getVariableLength();
    int32_t   getNextNBytesAsInt(int8_t numberOfBytes, boolean isDataByte = false);
    byte      getPunyNibbleOf(byte thisByte);
    byte      getMightyNibbleOf(byte thisByte);


};

#endif
