#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QtGui>
#include "track.hpp"

class Widget : public QWidget
{
    Q_OBJECT
    QComboBox *track_combo;
    QComboBox *file_combo;
    QLabel* track_label;
    QLineEdit* file_edit;
    QLineEdit* index_edit;
    QLineEdit* performer_edit;
    QLineEdit* postgap_edit;
    QLineEdit* pregap_edit;
    QLineEdit* songwriter_edit;
    QLineEdit* title_edit;
    QString track_string;
public:
    Track track;
    Widget(QWidget *parent = 0);
public slots:
    void UpdateFromVar();
    void UpdateToVar();
    void SelectName();
};

#endif // WIDGET_HPP
