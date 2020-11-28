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
#include <QWebEngineView>

#include "math/polynomial/Polynomial.h"
#include "math/long/Long.h"
#include "math/R.h"
#include "math/long/mpz_wrapper.h"


solution_widget::solution_widget(QPixmap input_pic,QString latex,QString ascii,QWidget *parent) :
    input_image(input_pic),latex_text(latex),ascii_text(ascii),QWidget(parent),
    ui(new Ui::solution_widget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("CINF");
    init_window();

    steps=nullptr;
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

    solution_view=new QWebEngineView{ui->scrollArea};
    ui->scrollArea->setWidget(solution_view);
    display_answer("<h1>This is just a test</h1>\n"
                   "The solutions of the quadratic equation should be displayed if an internet connection is correctly established.<br>\n"
                   "$$x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a}$$");
    R a=R{new Long{mpz_wrapper("104582958742895672409674389674380")}};
    R b=R{new Long{mpz_wrapper("50439650486294754927689254342095842097638596739")}};
    qDebug()<<QString::fromStdString((a*b).to_string())<<"\n";
}

void solution_widget::method_dealer(int choice){
    bool valid_answer = false;
    QString answer;
    QPixmap answer_png;
    switch (choice) {
        case 0:
            //call method1 func
            valid_answer = true;
            answer = "$";
            this->display_answer(answer.toStdString());
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

    /*if (!valid_answer){
       QMessageBox::critical(this,"Error","Invalid Implementation: Please check your input!");
    }
    else{
        QLabel* new_step = new QLabel;
        new_step->setPixmap(answer_png);
        //qDebug()<<"hhhh";
        new_step->setAlignment(Qt::AlignLeft);
        scrollarea_layout->addWidget(new_step);
        update();
    }*/

}

void solution_widget::display_answer(string answer){
    QString qs="<html><head><script>MathJax = {tex: {inlineMath: [['$', '$'], ['\\\\(', '\\\\)']]},svg: {fontCache: 'global'}};</script><script type=\"text/javascript\" id=\"MathJax-script\" async src=\"https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-svg.js\"></script></head><body>\n";
    qs=qs+QString::fromStdString(answer);
    qs=qs+"</body></html>";
    solution_view->setHtml(qs);
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
