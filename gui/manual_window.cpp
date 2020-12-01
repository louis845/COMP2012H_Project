#include "manual_window.h"
#include "ui_manual_window.h"

manual_window::manual_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::manual_window)
{
    ui->setupUi(this);
}

manual_window::~manual_window()
{
    delete ui;
}
