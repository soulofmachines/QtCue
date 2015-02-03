#ifndef ELEM_HPP
#define ELEM_HPP

#include <QtGui>

QComboBox *file_list();
QComboBox *track_list();
QComboBox *genre_list();
QString GetFileName();
bool ParseMiddle(QString expstr, QString &var, QString line, int begin = 1);
bool ParseLast(QString expstr, QString &var, QString line);
bool ParseLast(QString expstr, QString &var, QString line, int begin);
bool MMSSFF_valid(QString line);
//QString MMSSFF_sum(QString line1, QString line2, bool &ok);
//QString MMSSFF_diff(QString line1, QString line2, bool &ok);

#endif // ELEM_HPP
