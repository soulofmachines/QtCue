#include <QtGui>
#include "elem.hpp"
#include "track.hpp"
#include "widget.hpp"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    QGridLayout* main_layout = new QGridLayout;
    QWidget* space = new QWidget;
    space->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);

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
    index0_edit = new QLineEdit ("");
    index1_edit = new QLineEdit ("");
    index0_check = new QCheckBox;
    index0_check->setText("index 00");
    index0_check->setChecked(false);

    index0_edit->setInputMask("99:99:99");
    index1_edit->setInputMask("99:99:99");

    connect(file_button,SIGNAL(clicked()),SLOT(SelectName()));
    connect(index0_edit,SIGNAL(textChanged(QString)),SLOT(IndexColor()));
    connect(index1_edit,SIGNAL(textChanged(QString)),SLOT(IndexColor()));

    main_layout->addWidget(track_label,0,0,1,3);
    main_layout->addWidget(track_combo,0,3,1,3);
    main_layout->addWidget(file_edit,1,0,1,2);
    main_layout->addWidget(file_button,1,2,1,2);
    main_layout->addWidget(file_combo,1,4,1,2);
    main_layout->addWidget(index0_check,2,0,1,1);
    main_layout->addWidget(index0_edit,2,1,1,1);
    main_layout->addWidget(new QLabel("Index 01"),2,2,1,1);
    main_layout->addWidget(index1_edit,2,3,1,1);
    main_layout->addWidget(new QLabel("Title"),3,0,1,1);
    main_layout->addWidget(title_edit,3,1,1,1);
    main_layout->addWidget(new QLabel("Performer"),3,2,1,1);
    main_layout->addWidget(performer_edit,3,3,1,1);
    main_layout->addWidget(new QLabel("Songwriter"),3,4,1,1);
    main_layout->addWidget(songwriter_edit,3,5,1,1);
    main_layout->addWidget(space,5,0,1,6);
    main_layout->addWidget(line,4,0,1,6);
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
    index0_check->setChecked(track.index0_bool);
    index0_edit->setText(track.index0);
    index1_edit->setText(track.index1);
}

void Widget::UpdateToVar() {
    track.mode = track_combo->currentText();
    track.file_name = file_edit->text();
    track.file_mode = file_combo->currentText();
    track.title = title_edit->text();
    track.performer = performer_edit->text();
    track.songwriter = songwriter_edit->text();
    track.index0_bool = index0_check->isChecked();
    track.index0 = index0_edit->text();
    track.index1 = index1_edit->text();
}

void Widget::SelectName() {
    track.file_name = GetFileName();
    UpdateFromVar();
}

void Widget::IndexColor() {
    QPalette warning = palette();
    warning.setColor(index0_edit->foregroundRole(), Qt::red);
    if (!MMSSFF_valid(index0_edit->text()))
        index0_edit->setPalette(warning);
    else
        index0_edit->setPalette(palette());
    if (!MMSSFF_valid(index1_edit->text()))
        index1_edit->setPalette(warning);
    else
        index1_edit->setPalette(palette());
}
