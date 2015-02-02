#ifndef ELEM_HPP
#define ELEM_HPP

#include <QtGui>

QComboBox *file_list();
QComboBox *track_list();
QComboBox *genre_list();
QString GetFileName();
void ParseMiddle(QString expstr, QString &var, QString line, int begin = 1);
void ParseLast(QString expstr, QString &var, QString line);
void ParseLast(QString expstr, QString &var, QString line, int begin);

#endif // ELEM_HPP
