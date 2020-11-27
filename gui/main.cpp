#include "begin_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    begin_widget w;
    w.show();
    return a.exec();
}
