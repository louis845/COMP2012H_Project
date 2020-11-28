#include "begin_widget.h"
#include "ui_begin_widget.h"
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QToolButton>
#include <QPainter>
#include "show_capture.h"
#include "solution_widget.h"
#include <QLineEdit>
#include "input_key.h"
#include <QDebug>

begin_widget::begin_widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::begin_widget)
{
    ui->setupUi(this);
    this -> setWindowTitle("CINF");
    //this->setWindowFlags(Qt::FramelessWindowHint);

    //set logo
    QLabel * logo = new QLabel;
    logo -> setParent(this);

    QPixmap logo_image("://rsc/transparent_logo.png");
    logo_image = logo_image.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    logo -> setPixmap(logo_image);
    logo -> setFixedSize(300,300);

    //set start screen capture button
    QToolButton * photo_btn = new QToolButton;
    photo_btn -> setParent(this);
    photo_btn -> setAutoRaise(true);

    QIcon myicon;
    myicon.addFile(tr("://rsc/transparent_camera.png"));
    photo_btn->setIcon(myicon);
    photo_btn->setIconSize(QSize(90,90));

    //click on the button to enter screen_capture window
    connect(photo_btn,&QToolButton::clicked,[=](){
        this->hide();

        input_key * input_window = new input_key;
        input_window->show();

        connect(input_window,&input_key::user_quit,[=](){this->show();});

        void(input_key:: *input_done)(string,string) = &input_key::user_input;

        connect(input_window,input_done,[=](string username_input,string password_input){

            username = username_input;
            password = password_input;

            CaptureAndSolve();

            connect(this,&begin_widget::do_again,[=](){
                this->show();
                no_key_needed = true;
            });
            connect(this,nullptr,nullptr);
        });

        if (no_key_needed) {
            input_window->close();
            CaptureAndSolve();
            connect(this,&begin_widget::do_again,[=](){
                this->show();
            });
        }


    });


    //set total layout
    QGridLayout *layout = new QGridLayout;
    layout -> addWidget(logo,0,1,Qt::AlignHCenter);
    layout -> addWidget(photo_btn,1,1,Qt::AlignHCenter);
    //layout -> addWidget(input_widget,1,1,1,2,Qt::AlignHCenter);
    QSpacerItem * spacer1 = new QSpacerItem(10,20);
    layout -> addItem(spacer1,2,1,Qt::AlignHCenter);
    layout -> setVerticalSpacing(0);
    layout -> setHorizontalSpacing(0);
    layout -> setContentsMargins(10, 0, 10, 90);

    this->setLayout(layout);

    //set background color in paintevent
    //currently disabled


}

void begin_widget::CaptureAndSolve(){

    window_capture * cap_window = new window_capture;
    void(window_capture:: *complete_cap_sig)(QPixmap) = &window_capture::sig_capture_completed;

    connect(cap_window,complete_cap_sig,[=](QPixmap cap_pic){

        show_capture * show_cap = new show_capture(cap_pic,username,password);
        show_cap->show();

        void(show_capture:: *user_input)(QPixmap,string,string) = &show_capture::sig_checked;
        connect(show_cap,user_input,[=](QPixmap capture,string get_latex,string get_ascii){

            //parser func

            QString latex = QString::fromStdString(get_latex);
            QString ascii = QString::fromStdString(get_ascii);

            solution_widget * solution_window = new solution_widget(capture,latex,ascii);
            solution_window->show();

            connect(solution_window,&solution_widget::finish_sig,[=](){
                solution_window->close();
                this->close();
            });

            connect(solution_window,&solution_widget::next_problem_sig,[=](){
                solution_window->close();
                emit do_again();
            });

        });


    });

}

void begin_widget::paintEvent(QPaintEvent *event)
{
    //Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());

}

begin_widget::~begin_widget()
{
    delete ui;
}
