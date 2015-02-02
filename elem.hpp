#ifndef ELEM_HPP
#define ELEM_HPP

#include <QtGui>

QComboBox *file_list();
QComboBox *track_list();
QComboBox *genre_list();
QString GetFileName();
QString ParseMiddle(QString line, int begin = 1);
QString ParseLast(QString line);
QString ParseLast(QString line, int begin);

#endif // ELEM_HPP
