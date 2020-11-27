#include "solution_widget.h"
#include "ui_solution_widget.h"
#include <QPainter>
#include <QMessageBox>

solution_widget::solution_widget(QPixmap input_pic,QString latex,QString ascii,QWidget *parent) :
    input_image(input_pic),latex_text(latex),ascii_text(ascii),QWidget(parent),
    ui(new Ui::solution_widget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
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


    connect(ui->method_1,&QPushButton::clicked,[=](){
        method_dealer(0);
    });

    connect(ui->method_2,&QPushButton::clicked,[=](){
        method_dealer(1);
    });

    connect(ui->method_3,&QPushButton::clicked,[=](){
        method_dealer(2);
    });


}

void solution_widget::method_dealer(int choice){
    bool valid_answer = false;
    QString answer;
    switch (choice) {
        case 0:
            //call method1 func
            valid_answer = true;
            answer = "1+1=2";
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
        QLabel* new_step = new QLabel(answer);
        new_step->setAlignment(Qt::AlignLeft);
        scrollarea_layout->addWidget(new_step);
        update();
    }

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
