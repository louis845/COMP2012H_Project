#include "input_key.h"
#include "ui_input_key.h"
#include <QMessageBox>

input_key::input_key(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::input_key)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("CINF");

    connect(ui->ok_btn,&QPushButton::clicked,[=](){

        QString user_qstr = ui->user_text->text();
        user = user_qstr.toStdString();
        QString password_qstr = ui->password_text->text();
        password = password_qstr.toStdString();
        if (user.empty() || password.empty()){
            QMessageBox::warning(this,"Warning","The input must not be empty");
        }
        else{
            this->close();
            emit user_input(user,password);
        }

    });

    connect(ui->cancel_btn,&QPushButton::clicked,[=](){this->close();emit user_quit();});

}

input_key::~input_key()
{
    delete ui;
}
