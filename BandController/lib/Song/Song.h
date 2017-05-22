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

    // Setter
    void      setPath(String path);
    void      setTitle(String title);
    void      setLength(uint16_t length);

    void      setFormat(uint16_t format);
    void      setNtrks(uint16_t ntrks);
    void      setTpqn(uint16_t tpqn);
    void      setUsPerMidiQn(uint32_t usPerMidiQn);

    // Getter
    String    getInfo();

    String    getPath();
    String    getTitle();
    uint16_t  getLength();

    uint16_t  getFormat();
    uint16_t  getNtrks();
    uint16_t  getTpqn();
    uint32_t  getUsPerMidiQn();

  private:
    String    _path;
    String    _title;
    uint16_t  _length;

    uint16_t  _format;       // midi format
    uint16_t  _ntrks;        // number of tracks
    uint16_t  _tpqn;         // ticks per quoter note
    uint32_t  _usPerMidiQn;  // micro second per midi quoter note
};

#endif
