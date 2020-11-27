#include "solution_widget.h"
#include "ui_solution_widget.h"
#include <QPainter>
#include <QMessageBox>
#include <QMenu>
#include <fstream>
#include <iostream>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QApplication>


solution_widget::solution_widget(QPixmap input_pic,QString latex,QString ascii,QWidget *parent) :
    input_image(input_pic),latex_text(latex),ascii_text(ascii),QWidget(parent),
    ui(new Ui::solution_widget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("CINF");
    init_window();
}

void solution_widget::init_window(){
    input_image = input_image.scaled(330,160,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    ui->image_label->setPixmap(input_image);
    ui->image_label -> setFixedSize(330,160);
    ui->image_label->setBackgroundRole(QPalette::Mid);

    //ui->scrollArea->setAlignment(Qt::AlignTop);
    ui->scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    //ui->scrollArea->setWidgetResizable(true);
    scrollarea_layout = new QVBoxLayout(this);
    scrollarea_widget = new QWidget(this);
    scrollarea_widget->setLayout(scrollarea_layout);
    ui->scrollArea->setWidget(scrollarea_widget);
    //widget = new QWidget(this);
    //this->scrollarea_layout->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);

    ui->latex_textedit->setText(latex_text);
    ui->ascii_textedit->setText(ascii_text);

    connect(ui->latex_textedit,&QTextEdit::textChanged,[=](){
        latex_afteredit = ui->latex_textedit->toPlainText();
    });

    connect(ui->ascii_textedit,&QTextEdit::textChanged,[=](){
        ascii_afteredit = ui->ascii_textedit->toPlainText();
    });


    QMenu * method_menu = new QMenu(this);
    QAction * method1_act = new QAction("method1",this);
    QAction * method2_act = new QAction("method2",this);
    QAction * method3_act = new QAction("method3",this);
    method_menu->addAction(method1_act);
    method_menu->addAction(method2_act);
    method_menu->addAction(method3_act);
    ui->methods_btn->setMenu(method_menu);

    connect(method1_act,&QAction::triggered,[=](){
        method_dealer(0);
    });

    connect(method2_act,&QAction::triggered,[=](){
        method_dealer(1);
    });

    connect(method3_act,&QAction::triggered,[=](){
        method_dealer(2);
    });

    connect(ui->finish_btn,&QPushButton::clicked,[=](){emit finish_sig();});

    connect(ui->next_btn,&QPushButton::clicked,[=](){emit next_problem_sig();});


}

void solution_widget::method_dealer(int choice){
    bool valid_answer = false;
    QString answer;
    QPixmap answer_png;
    switch (choice) {
        case 0:
            //call method1 func
            valid_answer = true;
            answer = "$\\frac{11}{12}$";
            QPixmap* answer_png;
            answer_png = this->display_answer(answer.toStdString());
            qDebug()<<QDir::currentPath();
            break;
        case 1:
            //call method2 func
            break;
        case 2:
            //call method3 func
            break;
        default:
            break;
    }

    if (!valid_answer){
       QMessageBox::critical(this,"Error","Invalid Implementation: Please check your input!");
    }
    else{
        QLabel* new_step = new QLabel;
        new_step->setPixmap(answer_png);
        //qDebug()<<"hhhh";
        new_step->setAlignment(Qt::AlignLeft);
        scrollarea_layout->addWidget(new_step);
        update();
    }

}

QPixmap* solution_widget::display_answer(string answer){
    qDebug()<<"testtest\n";
    ofstream outfile;
    outfile.open("/Users/zhangjian/Desktop/COMP2012H/COMP2012H_Project/2012h project/AFHUIRSHGUIR.tex");
    outfile << "\\documentclass[preview]{standalone}";
    outfile << "\\usepackage{amsmath}";
    outfile << "\\begin{document}\n";
    outfile << answer;
    outfile << "\n\\end{document}";
    outfile.close();
    QProcess::execute("latex /Users/zhangjian/Desktop/COMP2012H/COMP2012H_Project/2012h project/AFHUIRSHGUIR.tex");
    QProcess::execute("dvipng /Users/zhangjian/Desktop/COMP2012H/COMP2012H_Project/2012h project/AFHUIRSHGUIR.dvi -D -2000");
    return new QPixmap{"/Users/zhangjian/Desktop/COMP2012H/COMP2012H_Project/2012h project/AFHUIRSHGUIR.png"};

}

void solution_widget::paintEvent(QPaintEvent *event)
{
    //Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());

}

solution_widget::~solution_widget()
{
    delete ui;
}
