#ifndef TRACK_HPP
#define TRACK_HPP

#include <QString>

class Track {
public:
    int number;
    bool index0_bool;
    QString mode, file_name, file_mode, title, performer, songwriter, isrc, index0, index1;
    Track() {
        number = 0;
        mode = "AUDIO";
        file_name = "";
        file_mode = "WAVE";
        title = "";
        performer = "";
        songwriter = "";
        isrc = "";
        index0 = "00:00:00";
        index1 = "00:00:00";
        index0_bool = false;
    }
};

#endif // TRACK_HPP
