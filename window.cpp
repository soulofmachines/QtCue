#include "elem.hpp"
#include "window.hpp"

Window::Window(QWidget *parent) : QWidget(parent)
{
    QWidget* space_left = new QWidget;
    space_left->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QWidget* space_right = new QWidget;
    space_right->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QHBoxLayout* window_layout = new QHBoxLayout;
    QGridLayout* left_layout = new QGridLayout;
    QVBoxLayout* right_layout = new QVBoxLayout;
    QVBoxLayout* track_box = new QVBoxLayout;
    QGridLayout* adddel_layout = new QGridLayout;
    QScrollArea* track_scroll = new QScrollArea;
    QWidget* track_widget = new QWidget;

    track_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    track_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    track_scroll->setWidgetResizable(true);

    QPushButton* load_button = new QPushButton("Load");
    QPushButton* save_button = new QPushButton("Save");
    QPushButton* file_button = new QPushButton ("...");
    QPushButton* add_button = new QPushButton("+");
    QPushButton* del_button = new QPushButton("-");

    track_layout = new QVBoxLayout;

    catalog_edit = new QLineEdit ("");
    file_edit = new QLineEdit ("");
    file_combo = file_list();
    title_edit = new QLineEdit ("");
    performer_edit = new QLineEdit ("");
    songwriter_edit = new QLineEdit ("");
    date_edit = new QLineEdit ("");
    genre_combo = genre_list();

    date_edit->setInputMask("9999");
    catalog_edit->setInputMask("9999999999999");

    catalog = "";
    file_name = "";
    file_mode = "WAVE";
    title = "";
    performer = "";
    songwriter = "";
    date = "";
    genre = "";

    warning = palette();
    warning.setColor(catalog_edit->foregroundRole(), Qt::red);
    warning.setColor(date_edit->foregroundRole(), Qt::red);

    connect(save_button,SIGNAL(clicked()),SLOT(Save()));
    connect(load_button,SIGNAL(clicked()),SLOT(Load()));
    connect(add_button,SIGNAL(clicked()),SLOT(AddWidget()));
    connect(del_button,SIGNAL(clicked()),SLOT(DelWidget()));
    connect(file_button,SIGNAL(clicked()),SLOT(SelectName()));
    connect(catalog_edit,SIGNAL(textChanged(QString)),SLOT(InputColor()));
    connect(date_edit,SIGNAL(textChanged(QString)),SLOT(InputColor()));

    left_layout->addWidget(load_button,0,0,1,3);
    left_layout->addWidget(save_button,0,3,1,3);
    left_layout->addWidget(new QLabel("FILE"),1,0,1,1);
    left_layout->addWidget(file_edit,1,1,1,3);
    left_layout->addWidget(file_button,1,4,1,1);
    left_layout->addWidget(file_combo,1,5,1,1);
    left_layout->addWidget(new QLabel("TITLE"),2,0,1,1);
    left_layout->addWidget(title_edit,2,1,1,5);
    left_layout->addWidget(new QLabel("PERFORMER"),3,0,1,1);
    left_layout->addWidget(performer_edit,3,1,1,5);
    left_layout->addWidget(new QLabel("SONGWRITER"),4,0,1,1);
    left_layout->addWidget(songwriter_edit,4,1,1,5);
    left_layout->addWidget(new QLabel("CATALOG"),5,0,1,1);
    left_layout->addWidget(catalog_edit,5,1,1,5);
    left_layout->addWidget(new QLabel("DATE"),6,0,1,1);
    left_layout->addWidget(date_edit,6,1,1,1);
    left_layout->addWidget(new QLabel("GENRE"),6,3,1,1);
    left_layout->addWidget(genre_combo,6,4,1,2);
    left_layout->addWidget(space_left,7,0,1,6);

    adddel_layout->addWidget(add_button);
    adddel_layout->addWidget(del_button);

    track_box->addLayout(track_layout);
    track_box->addWidget(space_right);
    track_widget->setLayout(track_box);
    track_scroll->setWidget(track_widget);
    right_layout->addLayout(adddel_layout);
    right_layout->addWidget(track_scroll);

    window_layout->addLayout(left_layout);
    window_layout->addLayout(right_layout);

    AddWidget();
    UpdateFromVar();
    track_scroll->setMinimumWidth(QApplication::desktop()->screenGeometry().width()*0.25);
    setFixedHeight(QApplication::desktop()->screenGeometry().height()*0.8);
    setLayout(window_layout);
}

void Window::AddWidget() {
    widget.push_back(new Widget());
    track_layout->addWidget(widget.back());
    if (widget.size() == 1)
        widget.back()->track.number = 1;
    else
        widget.back()->track.number = widget.at(widget.size()-2)->track.number+1;
    widget.back()->UpdateFromVar();
}

void Window::DelWidget() {
    if (widget.isEmpty())
        return;
    track_layout->removeWidget(widget.back());
    delete widget.back();
    widget.pop_back();
}

void Window::Load() {
    QString pregap = "";
    QString line;
    QString number;
    QTextStream stream;
    QMessageBox* pregap_message = new QMessageBox("Message","",QMessageBox::Question,QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton);
    QMessageBox* pregap_message_ok = new QMessageBox("Message","Apply Success",QMessageBox::Information,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    QMessageBox* pregap_message_nook = new QMessageBox("Message","Errors Occurred",QMessageBox::Warning,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    int pregap_answer = QMessageBox::No;
    bool pregap_ok = false;
    file.setFileName(QFileDialog::getOpenFileName(this,"Load","cdda.cue","Cue sheet (*.cue)"));
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen())
        return;
    while (!widget.isEmpty())
        DelWidget();
    stream.setDevice(&file);
    while (!stream.atEnd())
    {
        line = stream.readLine().trimmed();
        ParseLast("^CATALOG *",catalog,line);
        ParseMiddle("^FILE *",file_name,line);
        ParseLast("^FILE *",file_mode,line);
        ParseLast("^TITLE *",title,line,1);
        ParseLast("^PERFORMER *",performer,line,1);
        ParseLast("^SONGWRITER *",songwriter,line,1);
        ParseLast("^REM DATE *",date,line);
        ParseLast("^REM GENRE *",genre,line);
        if (line.contains(QRegExp("^TRACK *")))
        {
            AddWidget();
            ParseMiddle("^TRACK *", number, line);
            widget.back()->track.number = number.toInt();
            ParseLast("^TRACK *", widget.back()->track.mode, line);
            while (!stream.atEnd())
            {
                line = stream.readLine().trimmed();
                ParseMiddle("^FILE *", widget.back()->track.file_name, line);
                ParseLast("^FILE *", widget.back()->track.file_mode, line);
                ParseLast("^TITLE *", widget.back()->track.title, line, 1);
                ParseLast("^PERFORMER *", widget.back()->track.performer, line, 1);
                ParseLast("^SONGWRITER *", widget.back()->track.songwriter, line, 1);
                ParseLast("^ISRC *", widget.back()->track.isrc, line, 1);
                ParseLast("^PREGAP *", pregap, line, 1);
                if (line.contains(QRegExp("^INDEX 00 *")))
                    widget.back()->track.index0_bool = ParseLast("^INDEX 00 *", widget.back()->track.index0, line, 2);
                ParseLast("^INDEX 01 *", widget.back()->track.index1, line, 2);
                if (line.contains(QRegExp("^TRACK *")))
                {
                    widget.back()->UpdateFromVar();
                    AddWidget();
                    ParseMiddle("^TRACK *", number, line);
                    widget.back()->track.number = number.toInt();
                    ParseLast("^TRACK *", widget.back()->track.mode, line);
                }
            }
            widget.back()->UpdateFromVar();
        }
    }
    UpdateFromVar();
    file.close();
    if (!pregap.isEmpty()) {
        pregap_message->setText("Found PREGAP " + pregap + "\r\nApply?");
        pregap_answer = pregap_message->exec();
        if (pregap_answer == QMessageBox::Yes) {
            pregap_ok = ApplyPregap(pregap);
            if (pregap_ok)
                pregap_message_ok->exec();
            else
                pregap_message_nook->exec();
        }
        pregap.clear();
    }
}

void Window::Save(){
    if (widget.isEmpty())
        return;
    file.setFileName(QFileDialog::getSaveFileName(this,"Save","cdda.cue","Cue sheet (*.cue)"));
    file.open(QIODevice::Truncate|QIODevice::WriteOnly);
    if (!file.isOpen())
        return;
    UpdateToVar();
    if (!catalog.isEmpty())
        file.write("CATALOG " + catalog.toUtf8() + "\r\n");
    if (!file_name.isEmpty())
        file.write("FILE \"" + file_name.toUtf8() + "\" " + file_mode.toUtf8() + "\r\n");
    if (!title.isEmpty())
        file.write("TITLE \"" + title.toUtf8() + "\"\r\n");
    if (!performer.isEmpty())
        file.write("PERFORMER \"" + performer.toUtf8() + "\"\r\n");
    if (!songwriter.isEmpty())
        file.write("SONGWRITER \"" + songwriter.toUtf8() + "\"\r\n");
    if (!date.isEmpty())
        file.write("REM DATE " + date.toUtf8() + "\r\n");
    if (genre != "")
        file.write("REM GENRE \"" + genre.toUtf8() + "\"\r\n");
    for (int x = 0; x < widget.size(); ++x) {
        widget.at(x)->UpdateToVar();
        if (widget.at(x)->track.number < 10) {
            file.write("  TRACK 0" + QString::number(widget.at(x)->track.number).toUtf8() + " " + widget.at(x)->track.mode.toUtf8() + "\r\n");
        } else {
            file.write("  TRACK " + QString::number(widget.at(x)->track.number).toUtf8() + " " + widget.at(x)->track.mode.toUtf8() + "\r\n");
        }
        if (!widget.at(x)->track.file_name.isEmpty())
            file.write("    FILE \"" + widget.at(x)->track.file_name.toUtf8() + "\" " + widget.at(x)->track.file_mode.toUtf8() + "\r\n");
        if (!widget.at(x)->track.title.isEmpty())
            file.write("    TITLE \"" + widget.at(x)->track.title.toUtf8() + "\"\r\n");
        if (!widget.at(x)->track.performer.isEmpty())
            file.write("    PERFORMER \"" + widget.at(x)->track.performer.toUtf8() + "\"\r\n");
        if (!widget.at(x)->track.songwriter.isEmpty())
            file.write("    SONGWRITER \"" + widget.at(x)->track.songwriter.toUtf8() + "\"\r\n");
        if (!widget.at(x)->track.isrc.isEmpty())
            file.write("    ISRC " + widget.at(x)->track.isrc.toUtf8() + "\r\n");
        if (widget.at(x)->track.index0_bool)
        file.write("    INDEX 00 " + widget.at(x)->track.index0.toUtf8() + "\r\n");
        file.write("    INDEX 01 " + widget.at(x)->track.index1.toUtf8() + "\r\n");
    }
    file.close();
}

void Window::UpdateFromVar() {
    file_edit->setText(file_name);
    file_combo->setCurrentIndex(file_combo->findText(file_mode,Qt::MatchFixedString));
    title_edit->setText(title);
    performer_edit->setText(performer);
    songwriter_edit->setText(songwriter);
    catalog_edit->setText(catalog);
    genre_combo->setCurrentIndex(genre_combo->findText(genre,Qt::MatchFixedString));
    if (genre_combo->currentText() != genre)
    {
        genre_combo->setItemText(0,genre);
        genre_combo->setCurrentIndex(0);
    } else
        genre_combo->setItemText(0,"");
    date_edit->setText(date);
}

void Window::UpdateToVar() {
    file_name = file_edit->text();
    file_mode = file_combo->currentText();
    title = title_edit->text();
    performer = performer_edit->text();
    songwriter = songwriter_edit->text();
    catalog = catalog_edit->text();
    date = date_edit->text();
    genre = genre_combo->currentText();
}

void Window::SelectName() {
    file_name = GetFileName();
    UpdateFromVar();
}

void Window::InputColor() {
    if (catalog_edit->hasAcceptableInput())
        catalog_edit->setPalette(palette());
    else
        catalog_edit->setPalette(warning);
    if (date_edit->hasAcceptableInput())
        date_edit->setPalette(palette());
    else
        date_edit->setPalette(warning);
}

bool Window::ApplyPregap(QString pregap) {
    bool ok = true;
    if (!MMSSFF_valid(pregap))
        return false;
    if (widget.at(0)->track.index1 != "00:00:00")
        return false;
    widget.at(0)->track.index0_bool = true;
    widget.at(0)->track.index0 = widget.at(0)->track.index1;
    widget.at(0)->track.index1 = pregap;
    for (int x = 1; x < widget.size(); ++x) {
        if (!MMSSFF_valid(widget.at(x)->track.index0))
            return false;
        if (!MMSSFF_valid(widget.at(x)->track.index1))
            return false;
        widget.at(x)->track.index0 = MMSSFF_sum(widget.at(x)->track.index0, pregap, ok);
        if (!ok)
            return false;
        widget.at(x)->track.index1 = MMSSFF_sum(widget.at(x)->track.index1, pregap, ok);
        if (!ok)
            return false;
    }
    for (int x = 0; x < widget.size(); ++x)
        widget.at(x)->UpdateFromVar();
    return true;
}
