#ifndef SOLUTION_WIDGET_H
#define SOLUTION_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <string>
#include <QWebEngineView>
#include <atomic>

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
    void engine_choice_dealer(int choice);
    void display_answer(string answer);
    void display_preview(string preview);
    void display_preview_ascii(string preview);

    void captureMathExpression();

    void handle_ascii_update();
    void handle_ascii_update_async(string ascii_text);

    void run_solver();
    void run_solver_async();

    void fetch_async_loop();

    void navigatePrev();
    void navigateNext();
    void updateAnsDisp();
    void setNewSteps(StepsHistory* s);

    void receiveImage(QPixmap p);
    void on_treeWidget_itemPressed(QTreeWidgetItem *item);

    void closeEvent(QCloseEvent *event) override;
private:
    Ui::solution_widget *ui;

    string username;
    string password;
    string cur_input;
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
    int selected_engine = 0;

    std::vector<StepsHistory*> all_steps_list;

    /**
    * Run parser async
    */
    atomic<string*>new_intepret_ptr;
    atomic<string*>new_intepret_err;
    atomic<bool>ascii_or_latex; //true for latex, false for ascii

    atomic<bool>running_parser;

    /**
    * Run lin ops async
    */
    atomic<StepsHistory*>new_step_ptr;
    atomic<bool>running;

    bool running_handled; //If the async tasks are handled by the main thread.
signals:
    void finish_sig();
    void next_problem_sig();


public slots:



private slots:

};

#endif // SOLUTION_WIDGET_H
