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
#include <QDebug>

begin_widget::begin_widget(string username,string password,QWidget *parent)
    : QWidget(parent) , username(username), password(password)
    , ui(new Ui::begin_widget)
{
    ui->setupUi(this);
    this -> setWindowTitle("CINF");
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags((Qt::CustomizeWindowHint | Qt::WindowTitleHint) & ~Qt::WindowCloseButtonHint);

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

    begin_widget *pass_to_func=this;

    //click on the button to enter screen_capture window
    connect(photo_btn,&QToolButton::clicked,[=](){
        this->hide();
        QTimer *timer=new QTimer;
        timer->setSingleShot(true);
        QObject::connect(timer, &QTimer::timeout, [=](){
            delete timer;
            CaptureAndSolve();
        });
        timer->start(500);
    });

    connect(this,&begin_widget::do_again,[=](){
        this->show();
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

        show_capture * show_cap = new show_capture(cap_pic);
        show_cap->show();

        void(show_capture:: *user_input)(QPixmap) = &show_capture::sig_checked;
        connect(show_cap,user_input,[=](QPixmap capture){
            solution_widget *mparent=dynamic_cast<solution_widget*>(parent_window);
            mparent->receiveImage(capture);
        });

        connect(show_cap,&show_capture::sig_redo,[=](){
            begin_widget::setVisible(true);
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
