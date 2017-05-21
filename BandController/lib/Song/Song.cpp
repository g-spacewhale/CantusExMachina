/* ------------------------------------------------- */
//
//        BandController - Class for the Songs
//         created by Lukas Kaiser (mr14b042)
//                     May 2017
//
/* ------------------------------------------------- */


#include "Song.h"
#include <Arduino.h>


/*Song::Song(String path, String title, unsigned char format, uint16_t length)
{
  _path = path;
  _title = title;
  _format = format;
  _length = length;
}*/

Song::Song()
{
  _path = "NA";
  _title = "NA";
  _format = 10;
  _length = 0;
}

void Song::setPath(String path)
{
  _path = path;
}

String Song::getPath()
{
  return _path;
}

void Song::setTitle(String title)
{
  _title = title;
}

String Song::getTitle()
{
  return _title;
}

void Song::setFormat(unsigned char format)
{
  _format = format;
}

unsigned char Song::getFormat()
{
  return _format;
}

void Song::setLength(uint16_t length)
{
  _length = length;
}

uint16_t Song::getLength()
{
  return _length;
}

String Song::getInfo()
{
  return "Path: "+_path+" | Title: "+_title+" | Format: "+String(_format,DEC)+" | Length: "+String(_length,DEC);
}
