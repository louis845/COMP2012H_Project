#ifndef BEGIN_WIDGET_H
#define BEGIN_WIDGET_H

#include <QWidget>
#include "window_capture.h"

QT_BEGIN_NAMESPACE
namespace Ui { class begin_widget; }
QT_END_NAMESPACE

class begin_widget : public QWidget
{
    Q_OBJECT

public:
    begin_widget(QWidget *parent = nullptr);
    ~begin_widget();

    void paintEvent(QPaintEvent *);

private:
    Ui::begin_widget *ui;
};
#endif // BEGIN_WIDGET_H
