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
    Song();
    //Song(String path, String title, unsigned char format, uint16_t length);

    void setPath(String path);
    String getPath();
    void setTitle(String title);
    String getTitle();
    void setFormat(unsigned char format);
    unsigned char getFormat();
    void setLength(uint16_t length);
    uint16_t getLength();

    String getInfo();

  private:
    String _path;
    String _title;
    unsigned char _format;
    uint16_t _length;

};

#endif
