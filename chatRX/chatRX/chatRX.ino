#include <Arduino.h>

void setup()
{
  //pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(31250);
}

void loop()
{
  if (Serial1.available())
  {
    int inByte = Serial1.read();
    Serial.write(inByte);
    Serial.print("Such is the life of a Penis");
  }
}
