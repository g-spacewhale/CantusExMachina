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

// Setter
void      Song::setPath(String path) {_path = path;}
void      Song::setTitle(String title) {_title = title;}
void      Song::setLength(uint32_t length) {_length = length;}

void      Song::setFormat(uint16_t format) {_format = format;}
void      Song::setNtrks(uint16_t ntrks) {_ntrks = ntrks;}
void      Song::setTpqn(uint16_t tpqn) {_tpqn = tpqn;}
void      Song::setUsPerMidiQn(uint32_t usPerMidiQn) {_usPerMidiQn = usPerMidiQn;}

// Getter
String    Song::getPath() {return _path;}
String    Song::getTitle() {return _title;}
uint32_t  Song::getLength() {return _length;}

uint16_t  Song::getFormat() {return _format;}
uint16_t  Song::getNtrks(){return _ntrks;}
uint16_t  Song::getTpqn(){return _tpqn;}
uint32_t  Song::getUsPerMidiQn(){return _usPerMidiQn;}

String Song::getInfo()
{
  return "Path: "+_path+" | Title: "+_title+" | Format: "+String(_format,DEC)+" | Length: "+String(_length,DEC);
}
