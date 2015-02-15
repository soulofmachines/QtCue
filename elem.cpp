#include "elem.hpp"

QComboBox *file_list() {
    QStringList string = QStringList() << "BINARY" << "MOTOROLA"
                                         << "AIFF" << "WAVE" << "MP3";
    QComboBox* combo = new QComboBox();
    combo->addItems(string);
    return combo;
}

QComboBox *track_list() {
    QStringList string = QStringList() << "AUDIO" << "CDG"
                                         << "MODE1/2048" << "MODE1/2352"
                                         << "MODE2/2336" << "MODE2/2352"
                                         << "CDI/2336" << "CDI/2352";
    QComboBox* combo = new QComboBox();
    combo->addItems(string);
    return combo;
}

QComboBox *genre_list() {
    QStringList string = QStringList() << ""
                                       << "Blues"
                                       << "Classical"
                                       << "Country"
                                       << "Electronic"
                                       << "Folk"
                                       << "Funk"
                                       << "Hip-Hop"
                                       << "Jazz"
                                       << "Latin"
                                       << "New-Age"
                                       << "Pop"
                                       << "R&B"
                                       << "Reggae"
                                       << "Rock"
                                       << "Soundtrack";
    QComboBox* combo = new QComboBox();
    combo->addItems(string);
    combo->setEditable(true);
    return combo;
}

QString GetFileName() {
    QString file_name = QFileDialog::getOpenFileName(0,"Name","cdda.wav","Audio File (*.wav *.flac *.ape)");
    int begin = file_name.lastIndexOf(QDir::separator());
    if (begin == -1)
        begin = 0;
    return file_name.mid(begin+1);
}

bool ParseMiddle(QString expstr, QString &var, QString line, int begin) {
    QRegExp regexp;
    QStringList list;
    regexp.setPattern(expstr);
    if (line.contains(regexp))
    {
        list = line.split(" ",QString::SkipEmptyParts);
        line = list.at(begin);
        for (int x = begin+1; x < list.size()-1; ++x)
            line += " " + list.at(x);
        list = line.split("\"",QString::SkipEmptyParts);
        var = list.at(0);
        return true;
    } else
        return false;
}

bool ParseLast(QString expstr, QString &var, QString line) {
    QRegExp regexp;
    QStringList list;
    regexp.setPattern(expstr);
    if (line.contains(regexp))
    {
        list = line.split(" ",QString::SkipEmptyParts);
        line = list.back();
        list = line.split("\"",QString::SkipEmptyParts);
        var = list.at(0);
        return true;
    } else
        return false;
}

bool ParseLast(QString expstr, QString &var, QString line, int begin) {
    QRegExp regexp;
    QStringList list;
    regexp.setPattern(expstr);
    if (line.contains(regexp))
    {
        list = line.split(" ",QString::SkipEmptyParts);
        line = list.at(begin);
        for (int x = begin+1; x < list.size(); ++x)
            line += " " + list.at(x);
        list = line.split("\"",QString::SkipEmptyParts);
        var = list.at(0);
        return true;
    } else
        return false;
}

bool MMSSFF_valid(QString line) {
    if (line.count() != 8)
        return false;
    QStringList list = line.split(":",QString::SkipEmptyParts);
    if (list.size() != 3)
        return false;
    bool ok = true;
    int time = 0;
    time = list.at(0).toInt(&ok);
    if ((ok)&&(time>=0)&&(time<=99)) {
        time = list.at(1).toInt(&ok);
        if ((ok)&&(time>=0)&&(time<=59)) {
            time = list.at(2).toInt(&ok);
            if ((ok)&&(time>=0)&&(time<=74)) {
                return true;
            }
        }
    }
    return false;
}

QString MMSSFF_sum(QString line1, QString line2, bool &ok) {
    ok = true;
    QString retstr = "00:00:00";
    QStringList list1;
    QStringList list2;
    int mm = 0, ss = 0, ff = 0;
    if ((MMSSFF_valid(line1))&&(MMSSFF_valid(line2))) {
        list1 = line1.split(":",QString::SkipEmptyParts);
        list2 = line2.split(":",QString::SkipEmptyParts);
        mm = list1.at(0).toInt() + list2.at(0).toInt();
        ss = list1.at(1).toInt() + list2.at(1).toInt();
        ff = list1.at(2).toInt() + list2.at(2).toInt();
        if (ff > 74) {
            ff = ff - 75;
            ss = ss + 1;
        }
        if (ss > 59) {
            ss = ss - 60;
            mm = mm + 1;
        }
        if (mm > 99)
            ok = false;
        else {
            retstr.clear();
            if (mm < 10)
                retstr += "0";
            retstr += QString::number(mm) + ":";
            if (ss < 10)
                retstr += "0";
            retstr += QString::number(ss) + ":";
            if (ff < 10)
                retstr += "0";
            retstr += QString::number(ff);
        }
    } else
        ok = false;
    return retstr;
}

QString MMSSFF_diff(QString line1, QString line2, bool &ok) {
    ok = true;
    QString retstr = "00:00:00";
    QStringList list1;
    QStringList list2;
    int mm = 0, ss = 0, ff = 0;
    if ((MMSSFF_valid(line1))&&(MMSSFF_valid(line2))) {
        list1 = line1.split(":",QString::SkipEmptyParts);
        list2 = line2.split(":",QString::SkipEmptyParts);
        mm = list1.at(0).toInt() - list2.at(0).toInt();
        ss = list1.at(1).toInt() - list2.at(1).toInt();
        ff = list1.at(2).toInt() - list2.at(2).toInt();
        if (ff < 0) {
            ff = 75 + ff;
            ss = ss - 1;
        }
        if (ss < 0) {
            ss = 60 + ss;
            mm = mm - 1;
        }
        if (mm < 0)
            ok = false;
        else {
            retstr.clear();
            if (mm < 10)
                retstr = "0";
            retstr += QString::number(mm) + ":";
            if (ss < 10)
                retstr += "0";
            retstr += QString::number(ss) + ":";
            if (ff < 10)
                retstr += "0";
            retstr += QString::number(ff);
        }
    } else
        ok = false;
    return retstr;
}
