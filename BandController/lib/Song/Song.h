/* ------------------------------------------------- */
//
//        BandController - Class for the Songs
//         created by Lukas Kaiser (mr14b042)
//                     May 2017
//
/* ------------------------------------------------- */

#ifndef Song_h
#define Song_h

#include <Arduino.h>

class Song
{
  public:
    Song(String path, String title, char format, uint16_t length);
    String getInfo();

  private:
    String _path;
    String _title;
    char _format;
    uint16_t _length;

};

#endif
