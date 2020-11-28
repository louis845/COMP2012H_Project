#include "solution_widget.h"
#include "ui_solution_widget.h"
#include "begin_widget.h"
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


solution_widget::solution_widget(string username, string password, QWidget *parent) : QWidget(parent),
    ui(new Ui::solution_widget), parser("")
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("CINF");
    init_window();

    steps=nullptr;
}

void solution_widget::init_window(){


    connect(ui->plain_textedit,&QTextEdit::textChanged,this,&solution_widget::handle_plain_update);

    connect(ui->ascii_textedit,&QTextEdit::textChanged,this,&solution_widget::handle_ascii_update);

    connect(ui->scan_btn,&QPushButton::pressed,this,&solution_widget::captureMathExpression);

    ui->scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    scrollarea_layout = new QVBoxLayout(this);

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

    input_window=new begin_widget{username,password};
    input_window->setVisible(false);
    input_window->parent_window=this;
}

void solution_widget::handle_ascii_update(){
    QString text=ui->ascii_textedit->toPlainText();
    string std_text=text.toStdString();
    parser.reset_input(std_text);
    parser.parse();
}

void solution_widget::handle_plain_update(){

}

void solution_widget::captureMathExpression(){
    /*if(username==""){
        return;
    }*/

    input_window->show();
    this->setVisible(false);
}

void solution_widget::receiveImage(QPixmap p){
    this->setVisible(true);
    std::pair<string, string> result=Ocr::getInstance().request(p);
    string asciimath=result.second;
    ui->ascii_textedit->setText(QString::fromStdString(asciimath));
    handle_ascii_update();
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
    input_window->close();
    delete ui;
}
