#include <Arduino.h>
#include <MIDI.h>

#define pinSpeed 45
#define pinDirA 47
#define pinDirB 49

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void hit(int velocity);
void up();

void setup()
{
  pinMode(pinSpeed, OUTPUT);
  pinMode(pinDirA, OUTPUT);
  pinMode(pinDirB, OUTPUT);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop()
{
  MIDI.read();
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if(velocity > 0)
    hit(velocity);
  else
    up();
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  up();
}

void hit(int velocity)
{
  digitalWrite(pinDirA, LOW);
  digitalWrite(pinDirB, HIGH);

  analogWrite(pinSpeed, velocity * 2);
}

void up()
{
  digitalWrite(pinDirB, LOW);
  digitalWrite(pinDirA, HIGH);

  analogWrite(pinSpeed, 255);
}
