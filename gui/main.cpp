#include "input_key.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    input_key w;
    w.show();


    return a.exec();
}
