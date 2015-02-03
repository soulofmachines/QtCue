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
    QLineEdit* index1_edit;
    QLineEdit* performer_edit;
    QLineEdit* index0_edit;
    QLineEdit* songwriter_edit;
    QLineEdit* title_edit;
    QString track_string;
    QCheckBox* index0_check;
    QLabel* pregap_label;
public:
    Track track;
    Widget(QWidget *parent = 0);
public slots:
    void UpdateFromVar();
    void UpdateToVar();
    void SelectName();
    void IndexColor();
};

#endif // WIDGET_HPP
