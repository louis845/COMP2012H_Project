#include "show_capture.h"
#include "ui_show_capture.h"
#include <QPushButton>
#include <QPainter>
#include <QMessageBox>

show_capture::show_capture(QPixmap &capture_pic,string user,string password,QWidget *parent) :
    capture(capture_pic),user(user),password(password),QWidget(parent),
    ui(new Ui::show_capture)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("CINF");

    ui->show_pic->setPixmap(capture);

    connect(ui->ok,&QPushButton::clicked,[=](){

            bool valid_input = true;
            string latex = "latex";
            string ascii = "ascii";
            //parser func: input capture & username & password, get latex & ascii


            //warning
            if (!valid_input){
                QMessageBox::critical(this,"Error","Invalid username or password");
            }
            if (valid_input){
                this->close();
                emit sig_checked(capture,latex,ascii);
            }

    });


}


void show_capture::paintEvent(QPaintEvent *event)
{
    //Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());
/*
    QPainter painter_small(ui->lower_widget);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::lightGray);
    painter.drawRect(ui->lower_widget->rect());
*/
}

show_capture::~show_capture()
{
    delete ui;
}
