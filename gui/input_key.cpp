#include "input_key.h"
#include "ui_input_key.h"
#include "utils/ocr_api.h"
#include <QMessageBox>

void input_key::on_ok_button_clicked(){
    QString user_qstr = ui->user_text->text();
    user = user_qstr.toStdString();
    QString password_qstr = ui->password_text->text();
    password = password_qstr.toStdString();
    if (user.empty() || password.empty()){
        QMessageBox::warning(this,"Warning","The input must not be empty");
    }else{
        //check OCR api here
        Ocr::getInstance().setCredentials(user,password);
        bool err=false;
        try{
            Ocr::getInstance().testConnectivity();
        }catch (const std::runtime_error& exc){
            QMessageBox::warning(this,"Warning on initializing OCR",exc.what());
            err=true;
            // please show the error message i.e. err.what() to the user
        } catch (...) {
            QMessageBox::warning(this,"Warning on initializing OCR","Unknown error!");
            err=true;
        }
        if(!err){
            this->close();
            emit user_input(user,password);
        }
    }
}

input_key::input_key(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::input_key)
{
    ui->setupUi(this);

    this->setWindowTitle("CINF");

    connect(ui->ok_btn,&QPushButton::clicked,this,&input_key::on_ok_button_clicked);

    connect(ui->cancel_btn,&QPushButton::clicked,[=](){this->close(); emit user_quit();});
    connect(ui->no_ocr_btn,&QPushButton::clicked,[=](){
        this->close(); emit user_input("","");
    });
}

input_key::~input_key()
{
    delete ui;
}
