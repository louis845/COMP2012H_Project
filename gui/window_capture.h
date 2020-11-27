#ifndef WINDOW_CAPTURE_H
#define WINDOW_CAPTURE_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

class window_capture : public QWidget
{
    Q_OBJECT
public:
    explicit window_capture(QWidget *parent = nullptr);

private:
    QPixmap whole_Pixmap;
    QPixmap capture_Pixmap;
    int screen_width;
    int screen_height;
    QPainter cap_painter;
    QPoint begin_point;
    QPoint end_point;
    bool mouse_is_pressed;

    void initWindow();
    void loadBackgroundPixmap();
    QRect refine_Rect(const QPoint &beginPoint, const QPoint &endPoint);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);


signals:
    void sig_capture_completed(QPixmap captureImage);


};

#endif // WINDOW_CAPTURE_H
