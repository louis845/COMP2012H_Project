#include "window_capture.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QScreen>
#include <QRect>
#include <QMainWindow>


window_capture::window_capture(QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    initWindow();
    loadBackgroundPixmap();

}

void window_capture::initWindow(){
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowState(Qt::WindowActive | Qt::WindowFullScreen);
    this->show();
}

void window_capture::loadBackgroundPixmap(){

    QScreen *screen = QGuiApplication::primaryScreen();
    whole_Pixmap = screen -> grabWindow(QApplication::desktop()->winId());
    //whole_Pixmap = screen -> grabWindow(QApplication::desktop()->winId(),mapToGlobal(QPoint(0,0)).x(),mapToGlobal(QPoint(0,0)).y());
    screen_height = whole_Pixmap.width();
    screen_height = whole_Pixmap.height();
}

QRect window_capture::refine_Rect(const QPoint &beginPoint, const QPoint &endPoint){
    int x,y,width,height;
    x = (beginPoint.x() < endPoint.x()) ? beginPoint.x() : endPoint.x();
    y = (beginPoint.y() < endPoint.y()) ? beginPoint.y() : endPoint.y();
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());

    QRect refined_rect = QRect(x, y, width, height);
    if (refined_rect.width() == 0)
    {
        refined_rect.setWidth(1);
    }
    if (refined_rect.height() == 0)
    {
        refined_rect.setHeight(1);
    }

    return refined_rect;
}


void window_capture::mousePressEvent(QMouseEvent *event){
    if (event -> button() == Qt::LeftButton)
        {
            mouse_is_pressed = true;
            begin_point = event->pos();
            begin_point = mapToGlobal(begin_point);
        }

     return QWidget::mousePressEvent(event);
}

void window_capture::mouseMoveEvent(QMouseEvent* event)
{
    if (mouse_is_pressed)
    {
        end_point = event->pos();
        end_point = mapToGlobal(end_point);
        update();
    }
    return QWidget::mouseMoveEvent(event);
}


void window_capture::mouseReleaseEvent(QMouseEvent *event)
{
    end_point = event->pos();
    mouse_is_pressed = false;
    return QWidget::mouseReleaseEvent(event);
}

void window_capture::keyPressEvent(QKeyEvent *event)
{
    // press Esc to stop
    if (event->key() == Qt::Key_Escape)
    {
        this->setWindowState(Qt::WindowMinimized);
        this->close();
    }

    // press Eeter to complete
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        this->setWindowState(Qt::WindowMinimized);
        emit this->sig_capture_completed(capture_Pixmap);
        this->close();
    }
}

void window_capture::paintEvent(QPaintEvent *event)
{
    cap_painter.begin(this);

    QColor shadowColor = QColor(0, 0, 0, 100);
    cap_painter.setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
    cap_painter.drawPixmap(0, 0, whole_Pixmap);
    cap_painter.fillRect(whole_Pixmap.rect(), shadowColor);

    if (mouse_is_pressed)
    {
        QRect selectedRect = refine_Rect(begin_point,end_point);

        //int x = (begin_point.x() < end_point.x()) ? begin_point.x() : end_point.x();
        //int y = (begin_point.y() < end_point.y()) ? begin_point.y() : end_point.y();
        //capture_Pixmap = whole_Pixmap.copy(x,y,selectedRect.width(),selectedRect.height());

        capture_Pixmap = whole_Pixmap.copy(selectedRect);

        cap_painter.drawPixmap(selectedRect.topLeft(), capture_Pixmap);
        //cap_painter.drawRect(selectedRect);
    }

    cap_painter.end();
}











