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

#include "math/linear/LinearOperations.h"


solution_widget::solution_widget(string username, string password, QWidget *parent) : QWidget(parent),
    ui(new Ui::solution_widget), parser(""), username(username), password(password)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("CINF");
    init_window();
}

void solution_widget::init_window(){
    connect(ui->plain_textedit,&QPlainTextEdit::textChanged,this,&solution_widget::handle_plain_update);

    //connect(ui->ascii_textedit,&QPlainTextEdit::textChanged,this,&solution_widget::handle_ascii_update);

    connect(ui->scan_btn,&QPushButton::pressed,this,&solution_widget::captureMathExpression);

    connect(ui->previous_btn,&QPushButton::pressed,this,&solution_widget::navigatePrev);

    connect(ui->next_btn,&QPushButton::pressed,this,&solution_widget::navigateNext);

    connect(ui->next_btn2,&QPushButton::pressed,this,&solution_widget::handle_ascii_update);

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

    steps=new StepsHistory;
    steps->add_step(new StepText{"<h1>This is just a test</h1>\n"
                                 "The solutions of the quadratic equation should be displayed if an internet connection is correctly established.<br>\n"
                                 "$$x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a}$$\n"
                                 "<h1>Type something or scan a picture to start</h1>"});
    updateAnsDisp();
    input_window=new begin_widget{username,password};
    input_window->setVisible(false);
    input_window->parent_window=this;
}

void solution_widget::handle_ascii_update(){
    QString text=ui->ascii_textedit->toPlainText();
    string std_text=text.toStdString();
    parser.reset_input(std_text);
    const Info& i=parser.parse();
    qDebug()<<"engine used: "<<i.engine_used;
    qDebug()<<"success: "<<i.success;
    qDebug()<<"intepreted: "<<QString::fromStdString(i.interpreted_input);
    qDebug()<<"mat_size.size(): "<<i.mat_size.size();
    qDebug()<<"parsed_mat.size(): "<<i.parsed_mat.size();
    qDebug()<<QString::fromStdString(i.eval_result);
    qDebug()<<"----------------------------------------------------\n";
}

void solution_widget::handle_plain_update(){

}

void solution_widget::captureMathExpression(){
    if(username==""){
        return;
    }

    input_window->show();
    this->setVisible(false);
}

void solution_widget::receiveImage(QPixmap p){
    this->setVisible(true);
    std::pair<string, string> result=Ocr::getInstance().request(p);
    string asciimath=result.second;
    ui->ascii_textedit->setPlainText(QString::fromStdString(asciimath));
    handle_ascii_update();
}

void solution_widget::updateAnsDisp(){
    display_answer(steps->get_current_node().get_html_latex());
}

void solution_widget::navigateNext(){
    steps->next_node();
    updateAnsDisp();
}

void solution_widget::navigatePrev(){
    steps->previous_node();
    updateAnsDisp();
}

void solution_widget::setNewSteps(StepsHistory *new_step){
    delete steps;
    steps=new_step;
    updateAnsDisp();
}

void solution_widget::method_dealer(int choice){
    bool valid_answer = false;
    QString answer;
    QPixmap answer_png;
    switch (choice) {
        case 0:{
            //call method1 func
            valid_answer = true;
            answer = "$";
            this->display_answer(answer.toStdString());

            R **matrix=new R*[2];
            for(int i=0;i<2;i++){
                matrix[i]=new R[3];
                for(int j=0;j<3;j++){
                    matrix[i][j]=R{new Long{i+j}};
                }
            }
            StepsHistory* step;
            LinearOperationsFunc::invert(matrix,2,3,step);
            setNewSteps(step);
            for(int i=0;i<2;i++){
                delete[] matrix[i];
            }
            delete[]matrix;

            break;
        }
        case 1:{
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(-999, 999);

            R **matrix=new R*[4];
            for(int i=0;i<4;i++){
                matrix[i]=new R[5];
                for(int j=0;j<5;j++){
                    matrix[i][j]=R{new Long{dis(gen)}};
                }
            }
            StepsHistory* step;
            LinearOperationsFunc::solve(matrix,4,5,step);
            setNewSteps(step);
            for(int i=0;i<4;i++){
                delete[] matrix[i];
            }
            delete[]matrix;
            break;
        }
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
    qDebug().noquote();
    qDebug()<<qs;
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
    delete steps;
}
