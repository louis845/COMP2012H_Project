#include "input_key.h"
#include "solution_widget.h"

#include <QApplication>
#include <QObject>
#include <QDebug>
#include <QString>

void enter_solve_stage(string user, string password){
    solution_widget *w=new solution_widget{user, password};
    w->show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    input_key w;
    QObject::connect(&w, &input_key::user_quit, [=](){

    });

    QObject::connect(&w, &input_key::user_input, &enter_solve_stage);
    w.show();


    return a.exec();
}
