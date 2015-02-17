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
    QList<Track> tracks;
    QLineEdit* file_edit;
    QLineEdit* performer_edit;
    QLineEdit* title_edit;
    QLineEdit* songwriter_edit;
    QLineEdit* catalog_edit;
    QLineEdit* date_edit;
    QLineEdit* discid_edit;
    QLineEdit* comment_edit;
    QString catalog;
    QString file_name;
    QString file_mode;
    QString title;
    QString performer;
    QString songwriter;
    QString date;
    QString genre;
    QString discid;
    QString comment;
    QPalette warning;
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
    void InputColor();
    bool ApplyPregap(QString pregap);
    void SortTracks();
};

#endif // WINDOW_HPP
