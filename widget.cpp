#include <QtGui>
#include "elem.hpp"
#include "track.hpp"
#include "widget.hpp"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    QGridLayout* main_layout = new QGridLayout;
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);

    track = Track();
    QPushButton* track_inc = new QPushButton ("+");
    QPushButton* track_dec = new QPushButton ("-");
    performer_edit= new QLineEdit ("");
    songwriter_edit= new QLineEdit ("");
    title_edit= new QLineEdit ("");
    track_label = new QLabel(TrackFromNum(track.number));
    track_combo = track_list();
    file_edit = new QLineEdit ("");
    QPushButton* file_button = new QPushButton ("...");
    file_combo = file_list();
    isrc_edit = new QLineEdit ("");
    index0_edit = new QLineEdit ("");
    index1_edit = new QLineEdit ("");
    index0_check = new QCheckBox;
    index0_check->setText("INDEX 00");
    index0_check->setChecked(false);
    pregap_label = new QLabel;

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    isrc_edit->setInputMask(">NNNNN9999999");
    index0_edit->setInputMask("99:99:99");
    index1_edit->setInputMask("99:99:99");

    warning = palette();
    warning.setColor(isrc_edit->foregroundRole(), Qt::red);
    warning.setColor(index0_edit->foregroundRole(), Qt::red);
    warning.setColor(index1_edit->foregroundRole(), Qt::red);
    warning.setColor(index0_check->foregroundRole(), Qt::red);

    connect(track_inc,SIGNAL(clicked()),SLOT(TrackIncrease()));
    connect(track_dec,SIGNAL(clicked()),SLOT(TrackDecrease()));
    connect(file_button,SIGNAL(clicked()),SLOT(SelectName()));
    connect(isrc_edit,SIGNAL(textChanged(QString)),SLOT(InputColor()));
    connect(index0_edit,SIGNAL(textChanged(QString)),SLOT(InputColor()));
    connect(index1_edit,SIGNAL(textChanged(QString)),SLOT(InputColor()));
    connect(index0_check,SIGNAL(toggled(bool)),SLOT(InputColor()));

    main_layout->addWidget(line, 0,0,1,6);
    main_layout->addWidget(track_label,1,0,1,4);
    main_layout->addWidget(track_inc,1,4,1,1);
    main_layout->addWidget(track_dec,1,5,1,1);
    main_layout->addWidget(new QLabel("FILE"),2,0,1,1);
    main_layout->addWidget(file_edit,2,1,1,3);
    main_layout->addWidget(file_button,2,4,1,1);
    main_layout->addWidget(file_combo,2,5,1,1);
    main_layout->addWidget(new QLabel("TITLE"),3,0,1,1);
    main_layout->addWidget(title_edit,3,1,1,2);
    main_layout->addWidget(new QLabel("PERFORMER"),4,0,1,1);
    main_layout->addWidget(performer_edit,4,1,1,2);
    main_layout->addWidget(new QLabel("SONGWRITER"),5,0,1,1);
    main_layout->addWidget(songwriter_edit,5,1,1,2);
    main_layout->addWidget(new QLabel("ISRC"),6,0,1,1);
    main_layout->addWidget(isrc_edit,6,1,1,2);
    main_layout->addWidget(index0_check,3,3,1,1);
    main_layout->addWidget(index0_edit,3,4,1,2);
    main_layout->addWidget(new QLabel("INDEX 01"),4,3,1,1);
    main_layout->addWidget(index1_edit,4,4,1,2);
    main_layout->addWidget(pregap_label,5,3,1,3);
    setLayout(main_layout);
}

QString Widget::TrackFromNum(int num) {
    if (num < 10)
        return QString("TRACK 0" + QString::number(num));
    else
        return QString("TRACK " + QString::number(num));
}

void Widget::UpdateFromVar() {
    track_label->setText(TrackFromNum(track.number));
    track_combo->setCurrentIndex(track_combo->findText(track.mode,Qt::MatchFixedString));
    file_edit->setText(track.file_name);
    file_combo->setCurrentIndex(file_combo->findText(track.file_mode,Qt::MatchFixedString));
    title_edit->setText(track.title);
    performer_edit->setText(track.performer);
    songwriter_edit->setText(track.songwriter);
    isrc_edit->setText(track.isrc);
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
    track.isrc = isrc_edit->text();
    track.index0_bool = index0_check->isChecked();
    track.index0 = index0_edit->text();
    track.index1 = index1_edit->text();
}

void Widget::SelectName() {
    track.file_name = GetFileName();
    UpdateFromVar();
}

void Widget::InputColor() {
    bool ok = true;
    if (isrc_edit->hasAcceptableInput())
        isrc_edit->setPalette(palette());
    else
        isrc_edit->setPalette(warning);
    if (!MMSSFF_valid(index0_edit->text()))
        index0_edit->setPalette(warning);
    else
        index0_edit->setPalette(palette());
    if (!MMSSFF_valid(index1_edit->text()))
        index1_edit->setPalette(warning);
    else
        index1_edit->setPalette(palette());
    if (index0_check->isChecked()) {
        pregap_label->setText("PREGAP " + MMSSFF_diff(index1_edit->text(),index0_edit->text(),ok));
        if (ok)
            pregap_label->setPalette(palette());
        else
            pregap_label->setPalette(warning);
    } else
        pregap_label->setText("");
}

void Widget::TrackIncrease() {
    if (track.number < 99)
        track_label->setText(TrackFromNum(++track.number));
}

void Widget::TrackDecrease() {
    if (track.number > 1)
        track_label->setText(TrackFromNum(--track.number));
}
