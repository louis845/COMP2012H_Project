#ifndef SOLUTION_WIDGET_H
#define SOLUTION_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <string>
#include <QWebEngineView>

#include "steps/StepsHistory.h"
#include "utils/ocr_api.h"
#include "begin_widget.h"
#include "parser/parser.h"

using namespace std;

namespace Ui {
class solution_widget;
}

class solution_widget : public QWidget
{
    Q_OBJECT

public:
    explicit solution_widget(string username, string password,QWidget *parent = nullptr);
    ~solution_widget();
    void init_window();
    void paintEvent(QPaintEvent *event);
    void method_dealer(int choice);
    void display_answer(string answer);
    void captureMathExpression();

    void handle_plain_update();
    void handle_ascii_update();

    void receiveImage(QPixmap p);
private:
    Ui::solution_widget *ui;

    string username;
    string password;
    Parser parser;

    QVBoxLayout* scrollarea_layout;

    QWebEngineView *solution_view;
    QWebEngineView *intepretation_view;

    StepsHistory *steps;

    begin_widget *input_window;
signals:
    void finish_sig();
    void next_problem_sig();


public slots:



};

#endif // SOLUTION_WIDGET_H
