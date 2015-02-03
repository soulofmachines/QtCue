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
    QStringList string = QStringList() << "" << "GENRE1" << "GENRE2" << "GENRE3";
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
