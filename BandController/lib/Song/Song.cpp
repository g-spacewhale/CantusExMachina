/* ------------------------------------------------- */
//
//        BandController - Class for the Songs
//         created by Lukas Kaiser (mr14b042)
//                     May 2017
//
/* ------------------------------------------------- */


#include "Song.h"
#include <Arduino.h>


Song::Song(String path, String title, char format, uint16_t length)
{
  _path = path;
  _title = title;
  _format = format;
  _length = length;
}

String Song::getInfo()
{
  return "Path: "+_path+" | Title: "+_title+" | Format: "+String(_format,DEC)+" | Length: "+String(_length,DEC);
}
