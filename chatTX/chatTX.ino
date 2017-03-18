void setup()
{
  Serial.begin(9600);
  Serial1.begin(31250);
}

void loop()
{
  if (Serial.available())
  {
    int inByte = Serial.read();
    Serial1.write(inByte);
  }
}
