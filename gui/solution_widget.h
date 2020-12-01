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
#include <QTreeWidgetItem>

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
    void display_preview(string preview);
    void captureMathExpression();

    void handle_plain_update();
    void handle_ascii_update();

    void navigatePrev();
    void navigateNext();
    void updateAnsDisp();
    void setNewSteps(StepsHistory* s);

    void receiveImage(QPixmap p);
    void on_treeWidget_itemPressed(QTreeWidgetItem *item);
private:
    Ui::solution_widget *ui;

    string username;
    string password;
    Parser parser;

    QVBoxLayout* scrollarea_layout;

    QWebEngineView *solution_view;
    QWebEngineView *intepretation_view;

    StepsHistory *current_viewing_steps;

    begin_widget *input_window;

    QTimer* progress_timer;
    int currentValue = 0;

    int question_num = 0;

    int selected_choice;

    std::vector<StepsHistory*> all_steps_list;
signals:
    void finish_sig();
    void next_problem_sig();


public slots:



};

#endif // SOLUTION_WIDGET_H
