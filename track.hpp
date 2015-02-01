#ifndef TRACK_HPP
#define TRACK_HPP

#include <QString>

class Track {
public:
    int number;
    QString mode, file_name, file_mode, title, performer, songwriter, index, pregap, postgap;
    Track() {
        number = 0;
        mode = "AUDIO";
        file_name = "";
        file_mode = "WAVE";
        title = "";
        performer = "";
        songwriter = "";
        index = "00:00:00";
        pregap = "00:00:00";
        postgap = "00:00:00";
    }
};

#endif // TRACK_HPP
