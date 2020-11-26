#include "begin_widget.h"
#include "ui_begin_widget.h"
//#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QToolButton>
#include <QPainter>
#include "show_capture.h"
#include "solution_widget.h"

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
        window_capture * cap_window = new window_capture;
        void(window_capture:: *complete_cap_sig)(QPixmap) = &window_capture::sig_capture_completed;

        connect(cap_window,complete_cap_sig,[=](QPixmap cap_pic){

            show_capture * show_cap = new show_capture(cap_pic);
            show_cap->show();

            void(show_capture:: *user_input)(QPixmap,string,string) = &show_capture::sig_checked;
            connect(show_cap,user_input,[=](QPixmap capture,string get_latex,string get_ascii){

                //parser func

                QString latex = QString::fromStdString(get_latex);
                QString ascii = QString::fromStdString(get_ascii);

                solution_widget * solution_window = new solution_widget(capture,latex,ascii);
                solution_window->show();


            });


        });



    });

    //set layout
    QGridLayout *layout = new QGridLayout;
    layout -> addWidget(logo,0,1,Qt::AlignHCenter);
    layout -> addWidget(photo_btn,1,1,Qt::AlignHCenter);
    QSpacerItem * spacer1 = new QSpacerItem(10,20);
    layout -> addItem(spacer1,2,1,Qt::AlignHCenter);
    layout -> setVerticalSpacing(0);
    layout -> setHorizontalSpacing(0);
    layout -> setContentsMargins(10, 0, 10, 90);

    this->setLayout(layout);

    //set background color in paintevent
    //currently disabled


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
