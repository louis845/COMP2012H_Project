#ifndef MANUAL_WINDOW_H
#define MANUAL_WINDOW_H

#include <QWidget>

namespace Ui {
class manual_window;
}

class manual_window : public QWidget
{
    Q_OBJECT

public:
    explicit manual_window(QWidget *parent = nullptr);
    ~manual_window();

private:
    Ui::manual_window *ui;
};

#endif // MANUAL_WINDOW_H
