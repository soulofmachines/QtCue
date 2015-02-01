#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QtGui>
#include "widget.hpp"

class Window : public QWidget
{
    Q_OBJECT
    QFile file;
    QVBoxLayout* track_layout;
    QComboBox* file_combo;
    QComboBox* genre_combo;
    QVector<Widget*> widget;
    QLineEdit* file_edit;
    QLineEdit* performer_edit;
    QLineEdit* title_edit;
    QLineEdit* songwriter_edit;
    QLineEdit* date_edit;
    QString file_name;
    QString file_mode;
    QString title;
    QString performer;
    QString songwriter;
    QString date;
    QString genre;
public:
    Window(QWidget *parent = 0);
public slots:
    void AddWidget();
    void DelWidget();
    void Load();
    void Save();
    void UpdateFromVar();
    void UpdateToVar();
    void SelectName();
};

#endif // WINDOW_HPP
