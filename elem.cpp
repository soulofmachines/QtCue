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
