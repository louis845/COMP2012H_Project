#ifndef SOLUTION_WIDGET_H
#define SOLUTION_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <string>
using namespace std;

namespace Ui {
class solution_widget;
}

class solution_widget : public QWidget
{
    Q_OBJECT

public:
    explicit solution_widget(QPixmap input_pic,QString latex,QString ascii,QWidget *parent = nullptr);
    ~solution_widget();
    void init_window();
    void paintEvent(QPaintEvent *event);
    void method_dealer(int choice);
    QPixmap* display_answer(string answer);

private:
    Ui::solution_widget *ui;
    QPixmap input_image;

    QString latex_text;
    QString ascii_text;

    QString latex_afteredit;
    QString ascii_afteredit;

    QVBoxLayout* scrollarea_layout;
    QWidget* scrollarea_widget;

    //QList<QLabel> steps;

signals:
    void finish_sig();
    void next_problem_sig();


public slots:



};

#endif // SOLUTION_WIDGET_H
