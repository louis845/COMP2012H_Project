#include "dialog_key.h"
#include "ui_dialog_key.h"
#include <QLineEdit>
#include <QPushButton>

Dialog_key::Dialog_key(QPixmap &capture_pic,QWidget *parent) :
    capture(capture_pic),QDialog(parent),
    ui(new Ui::Dialog_key)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("CINF");

    connect(ui->ok_btn,&QPushButton::clicked,[=](){
        QString user_qstr = ui->user_text->text();
        user = user_qstr.toStdString();
        QString password_qstr = ui->password_text->text();
        password = password_qstr.toStdString();
        emit ok_cliked(user,password);
        this->close();
    });

    connect(ui->cancel_btn,&QPushButton::clicked,[=](){
        this->close();
    });

}



Dialog_key::~Dialog_key()
{
    delete ui;
}
