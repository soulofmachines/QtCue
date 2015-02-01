#include "elem.hpp"
#include "window.hpp"

Window::Window(QWidget *parent) : QWidget(parent)
{
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
    QTextStream stream;
    QRegExp regexp;
    QStringList list;
    //int widgets = 1;
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
        regexp.setPattern("^FILE *");
        if (line.contains(regexp))
        {
            list = line.split(" ",QString::SkipEmptyParts);
            file_name = list.at(1);
            for (int x = 2; x < list.size()-1; ++x)
                file_name += " " + list.at(x);
            file_mode = list.back();
            list = file_name.split("\"",QString::SkipEmptyParts);
            file_name = list.at(0);
        }
        regexp.setPattern("^TITLE *");
        if (line.contains(regexp))
        {
            list = line.split(" ",QString::SkipEmptyParts);
            title = list.at(1);
            for (int x = 2; x < list.size(); ++x)
                title += " " + list.at(x);
            list = title.split("\"",QString::SkipEmptyParts);
            title = list.at(0);
        }
        regexp.setPattern("^PERFORMER *");
        if (line.contains(regexp))
        {
            list = line.split(" ",QString::SkipEmptyParts);
            performer = list.at(1);
            for (int x = 2; x < list.size(); ++x)
               performer += " " + list.at(x);
            list = performer.split("\"",QString::SkipEmptyParts);
            performer = list.at(0);
        }
        regexp.setPattern("^SONGWRITER *");
        if (line.contains(regexp))
        {
            list = line.split(" ",QString::SkipEmptyParts);
            songwriter = list.at(1);
            for (int x = 2; x < list.size(); ++x)
                songwriter += " " + list.at(x);
            list = songwriter.split("\"",QString::SkipEmptyParts);
            songwriter = list.at(0);
        }
        regexp.setPattern("^REM DATE *");
        if (line.contains(regexp))
        {
            list = line.split(" ",QString::SkipEmptyParts);
            date = list.at(2);
            for (int x = 3; x < list.size(); ++x)
                date += " " + list.at(x);
            list = date.split("\"",QString::SkipEmptyParts);
            date = list.at(0);
        }
        regexp.setPattern("^REM GENRE *");
        if (line.contains(regexp))
        {
            list = line.split(" ",QString::SkipEmptyParts);
            genre = list.at(2);
            for (int x = 3; x < list.size(); ++x)
                genre += " " + list.at(x);
            list = genre.split("\"",QString::SkipEmptyParts);
            genre = list.at(0);
        }
        regexp.setPattern("^TRACK *");
        if (line.contains(regexp))
        {
            AddWidget();
            list = line.split(" ",QString::SkipEmptyParts);
            widget.back()->track.number = list.at(1).toInt();
            widget.back()->track.mode = list.at(2);
            while (!stream.atEnd())
            {
                line = stream.readLine().trimmed();
                regexp.setPattern("^FILE *");
                if (line.contains(regexp))
                {
                    list = line.split(" ",QString::SkipEmptyParts);
                    widget.back()->track.file_name = list.at(1);
                    for (int x = 2; x < list.size()-1; ++x)
                        widget.back()->track.file_name += " " + list.at(x);
                    widget.back()->track.file_mode = list.back();
                    list = widget.back()->track.file_name.split("\"",QString::SkipEmptyParts);
                    widget.back()->track.file_name = list.at(0);
                }
                regexp.setPattern("^TITLE *");
                if (line.contains(regexp))
                {
                    list = line.split(" ",QString::SkipEmptyParts);
                    widget.back()->track.title = list.at(1);
                    for (int x = 2; x < list.size(); ++x)
                        widget.back()->track.title += " " + list.at(x);
                    list = widget.back()->track.title.split("\"",QString::SkipEmptyParts);
                    widget.back()->track.title = list.at(0);
                }
                regexp.setPattern("^PERFORMER *");
                if (line.contains(regexp))
                {
                    list = line.split(" ",QString::SkipEmptyParts);
                    widget.back()->track.performer = list.at(1);
                    for (int x = 2; x < list.size(); ++x)
                       widget.back()->track.performer += " " + list.at(x);
                    list = widget.back()->track.performer.split("\"",QString::SkipEmptyParts);
                    widget.back()->track.performer = list.at(0);
                }
                regexp.setPattern("^SONGWRITER *");
                if (line.contains(regexp))
                {
                    list = line.split(" ",QString::SkipEmptyParts);
                    widget.back()->track.songwriter = list.at(1);
                    for (int x = 2; x < list.size(); ++x)
                        widget.back()->track.songwriter += " " + list.at(x);
                    list = widget.back()->track.songwriter.split("\"",QString::SkipEmptyParts);
                    widget.back()->track.songwriter = list.at(0);
                }
                regexp.setPattern("^INDEX 01 *");
                if (line.contains(regexp))
                {
                    list = line.split(" ",QString::SkipEmptyParts);
                    widget.back()->track.index = list.at(2);
                }
                regexp.setPattern("^PREGAP *");
                if (line.contains(regexp))
                {
                    list = line.split(" ",QString::SkipEmptyParts);
                    widget.back()->track.pregap = list.at(1);
                }
                regexp.setPattern("^POSTGAP *");
                if (line.contains(regexp))
                {
                    list = line.split(" ",QString::SkipEmptyParts);
                    widget.back()->track.postgap = list.at(1);
                }
                regexp.setPattern("^TRACK *");
                if (line.contains(regexp))
                {
                    widget.back()->UpdateFromVar();
                    AddWidget();
                    list = line.split(" ",QString::SkipEmptyParts);
                    widget.back()->track.number = list.at(1).toInt();
                    widget.back()->track.mode = list.at(2);
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
        if (widget.at(x)->track.pregap != "00:00:00")
            file.write("    PREGAP " + widget.at(x)->track.pregap.toUtf8() + "\r\n");
        file.write("    INDEX 01 " + widget.at(x)->track.index.toUtf8() + "\r\n");
        if (widget.at(x)->track.postgap != "00:00:00")
            file.write("    POSTGAP " + widget.at(x)->track.postgap.toUtf8() + "\r\n");
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


