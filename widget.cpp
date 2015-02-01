#include <QtGui>
#include "elem.hpp"
#include "track.hpp"
#include "widget.hpp"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    QGridLayout* main_layout = new QGridLayout;

    track = Track();
    performer_edit= new QLineEdit ("");
    songwriter_edit= new QLineEdit ("");
    title_edit= new QLineEdit ("");
    track_string = "Track " + QString::number(track.number);
    track_label = new QLabel(track_string);
    track_combo = track_list();
    file_edit = new QLineEdit ("");
    QPushButton* file_button = new QPushButton ("...");
    file_combo = file_list();
    pregap_edit = new QLineEdit ("");
    index_edit = new QLineEdit ("");
    postgap_edit = new QLineEdit ("");

    pregap_edit->setInputMask("99:99:99");
    index_edit->setInputMask("99:99:99");
    postgap_edit->setInputMask("99:99:99");

    connect(file_button,SIGNAL(clicked()),SLOT(SelectName()));

    main_layout->addWidget(track_label,0,0,1,3);
    main_layout->addWidget(track_combo,0,3,1,3);
    main_layout->addWidget(file_edit,1,0,1,2);
    main_layout->addWidget(file_button,1,2,1,2);
    main_layout->addWidget(file_combo,1,4,1,2);
    main_layout->addWidget(new QLabel("Index"),2,0,1,1);
    main_layout->addWidget(index_edit,2,1,1,1);
    main_layout->addWidget(new QLabel("Pregap"),2,2,1,1);
    main_layout->addWidget(pregap_edit,2,3,1,1);
    main_layout->addWidget(new QLabel("Postgap"),2,4,1,1);
    main_layout->addWidget(postgap_edit,2,5,1,3);
    main_layout->addWidget(new QLabel("Title"),3,0,1,1);
    main_layout->addWidget(title_edit,3,1,1,1);
    main_layout->addWidget(new QLabel("Performer"),3,2,1,1);
    main_layout->addWidget(performer_edit,3,3,1,1);
    main_layout->addWidget(new QLabel("Songwriter"),3,4,1,1);
    main_layout->addWidget(songwriter_edit,3,5,1,1);
    setLayout(main_layout);
}

void Widget::UpdateFromVar() {
    track_string = "Track " + QString::number(track.number);
    track_label->setText(track_string);
    track_combo->setCurrentIndex(track_combo->findText(track.mode,Qt::MatchFixedString));
    file_edit->setText(track.file_name);
    file_combo->setCurrentIndex(file_combo->findText(track.file_mode,Qt::MatchFixedString));
    title_edit->setText(track.title);
    performer_edit->setText(track.performer);
    songwriter_edit->setText(track.songwriter);
    index_edit->setText(track.index);
    pregap_edit->setText(track.pregap);
    postgap_edit->setText(track.postgap);
}

void Widget::UpdateToVar() {
    track.mode = track_combo->currentText();
    track.file_name = file_edit->text();
    track.file_mode = file_combo->currentText();
    track.title = title_edit->text();
    track.performer = performer_edit->text();
    track.songwriter = songwriter_edit->text();
    track.index = index_edit->text();
    track.pregap = pregap_edit->text();
    track.postgap = postgap_edit->text();
}

void Widget::SelectName() {
    track.file_name = GetFileName();
    UpdateFromVar();
}
