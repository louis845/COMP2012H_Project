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
    explicit show_capture(QPixmap &capture_pic,QWidget *parent = nullptr);
    ~show_capture();

    void paintEvent(QPaintEvent *event);

private:
    Ui::show_capture *ui;
    QPixmap capture;

signals:
    void sig_redo();
    void sig_checked(QPixmap catureImage);
};

#endif // SHOW_CAPTURE_H
