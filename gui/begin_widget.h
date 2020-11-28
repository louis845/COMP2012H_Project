#ifndef BEGIN_WIDGET_H
#define BEGIN_WIDGET_H

#include <QWidget>
#include "window_capture.h"
#include <string>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class begin_widget; }
QT_END_NAMESPACE

class begin_widget : public QWidget
{
    Q_OBJECT

public:
    begin_widget(QWidget *parent = nullptr);
    ~begin_widget();
    void CaptureAndSolve();

    void paintEvent(QPaintEvent *);

private:
    Ui::begin_widget *ui;
    string username;
    string password;
    bool no_key_needed = false;

signals:
    void do_again();
};
#endif // BEGIN_WIDGET_H
