#include "elem.hpp"
#include "window.hpp"

Window::Window(QWidget *parent) : QWidget(parent)
{
    QWidget* space = new QWidget;
    space->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QHBoxLayout* main_layout = new QHBoxLayout;
    QGridLayout* head_layout = new QGridLayout;
    QScrollArea* track_scroll = new QScrollArea;
    QWidget* track_widget = new QWidget;
    track_layout = new QVBoxLayout;

    file_edit = new QLineEdit ("");
    performer_edit = new QLineEdit ("");
    title_edit = new QLineEdit ("");
    songwriter_edit = new QLineEdit ("");
    date_edit = new QLineEdit ("");
    QPushButton* load_button = new QPushButton("Load");
    QPushButton* save_button = new QPushButton("Save");
    QPushButton* file_button = new QPushButton ("...");
    file_combo = file_list();
    genre_combo = genre_list();
    QPushButton* add_button = new QPushButton("+");
    QPushButton* del_button = new QPushButton("-");


    file_name = "";
    file_mode = "WAVE";
    title = "";
    performer = "";
    songwriter = "";
    date = "0000";
    genre = "";

    date_edit->setInputMask("9999");
    connect(save_button,SIGNAL(clicked()),SLOT(Save()));
    connect(load_button,SIGNAL(clicked()),SLOT(Load()));
    connect(add_button,SIGNAL(clicked()),SLOT(AddWidget()));
    connect(del_button,SIGNAL(clicked()),SLOT(DelWidget()));
    connect(file_button,SIGNAL(clicked()),SLOT(SelectName()));

    head_layout->addWidget(load_button,0,0,1,2);
    head_layout->addWidget(save_button,0,2,1,2);
    head_layout->addWidget(file_edit,1,0,1,2);
    head_layout->addWidget(file_button,1,2,1,1);
    head_layout->addWidget(file_combo,1,3,1,1);
    head_layout->addWidget(new QLabel("Performer"),2,0,1,1);
    head_layout->addWidget(performer_edit,2,1,1,3);
    head_layout->addWidget(new QLabel("Title"),3,0,1,1);
    head_layout->addWidget(title_edit,3,1,1,3);
    head_layout->addWidget(new QLabel("Songwriter"),4,0,1,1);
    head_layout->addWidget(songwriter_edit,4,1,1,3);
    head_layout->addWidget(new QLabel("Date"),5,0,1,1);
    head_layout->addWidget(date_edit,5,1,1,1);
    head_layout->addWidget(new QLabel("Genre"),5,2,1,1);
    head_layout->addWidget(genre_combo,5,3,1,1);
    head_layout->addWidget(space, 6,0,1,4);
    main_layout->addLayout(head_layout);
    main_layout->addWidget(track_scroll);
    track_layout->addWidget(add_button);
    track_layout->addWidget(del_button);
    track_widget->setLayout(track_layout);
    track_scroll->setWidget(track_widget);

    track_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    track_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    track_scroll->setWidgetResizable(true);

    AddWidget();
    UpdateFromVar();
    setFixedHeight(QApplication::desktop()->screenGeometry().height()*0.8);
    setLayout(main_layout);
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
    QString line;
    QString number;
    QTextStream stream;
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
}

void Window::Save(){
    if (widget.isEmpty())
        return;
    file.setFileName(QFileDialog::getSaveFileName(this,"Save","cdda.cue","Cue sheet (*.cue)"));
    file.open(QIODevice::Truncate|QIODevice::WriteOnly);
    if (!file.isOpen())
        return;
    UpdateToVar();
    if (!file_name.isEmpty())
        file.write("FILE \"" + file_name.toUtf8() + "\" " + file_mode.toUtf8() + "\r\n");
    if (!title.isEmpty())
        file.write("TITLE \"" + title.toUtf8() + "\"\r\n");
    if (!performer.isEmpty())
        file.write("PERFORMER \"" + performer.toUtf8() + "\"\r\n");
    if (!songwriter.isEmpty())
        file.write("SONGWRITER \"" + songwriter.toUtf8() + "\"\r\n");
    if (date != "0000")
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
    genre_combo->setCurrentIndex(genre_combo->findText(genre,Qt::MatchFixedString));
    date_edit->setText(date);
}

void Window::UpdateToVar() {
    file_name = file_edit->text();
    file_mode = file_combo->currentText();
    title = title_edit->text();
    performer = performer_edit->text();
    songwriter = songwriter_edit->text();
    date = date_edit->text();
    genre = genre_combo->currentText();
}

void Window::SelectName() {
    file_name = GetFileName();
    UpdateFromVar();
}


