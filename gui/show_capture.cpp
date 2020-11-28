#include "show_capture.h"
#include "ui_show_capture.h"
#include <QPushButton>
#include <QPainter>
#include <QMessageBox>

show_capture::show_capture(QPixmap &capture_pic,QWidget *parent) :
    capture(capture_pic),QWidget(parent),
    ui(new Ui::show_capture)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    this->setWindowFlags((Qt::CustomizeWindowHint | Qt::WindowTitleHint) & ~Qt::WindowCloseButtonHint);
    this->setWindowTitle("CINF");

    ui->show_pic->setPixmap(capture);

    connect(ui->ok,&QPushButton::clicked,[=](){
        this->close();
        emit sig_checked(capture);
    });

    connect(ui->again,&QPushButton::clicked,[=](){
        this->close();
        emit sig_redo();
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
