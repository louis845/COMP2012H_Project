#include "input_key.h"
#include "solution_widget.h"
#include "parser/parser.h"

#include <QApplication>
#include <QObject>
#include <QDebug>
#include <QString>

#include <string>

void enter_solve_stage(string user, string password){
    solution_widget *w=new solution_widget{user, password};
    w->show();
}

int main(int argc, char *argv[])
{
    /*
    Parser p{""};
    p.reset_input("[[3,4,4+t][5,6]]");
    p.parse();
    p.reset_input("[[3,4,4+t,],[5,6]]");
    p.parse();*/

    QApplication a(argc, argv);
    input_key w;
    QObject::connect(&w, &input_key::user_quit, [=](){

    });

    QObject::connect(&w, &input_key::user_input, &enter_solve_stage);
    w.show();


    return a.exec();
}
