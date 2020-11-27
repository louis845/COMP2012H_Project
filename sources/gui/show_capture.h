#ifndef SHOW_CAPTURE_H
#define SHOW_CAPTURE_H

#include <QWidget>
#include <QPixmap>
#include <string>
using namespace std;


namespace Ui {
class show_capture;
}

class show_capture : public QWidget
{
    Q_OBJECT

public:
    explicit show_capture(QPixmap &capture_pic,string user,string password,QWidget *parent = nullptr);
    ~show_capture();

    void paintEvent(QPaintEvent *event);

private:
    Ui::show_capture *ui;
    QPixmap capture;
    string user;
    string password;


signals:
    //void sig_redo();
    void sig_checked(QPixmap catureImage,string user,string password);
};

#endif // SHOW_CAPTURE_H
