#ifndef SOLUTION_WIDGET_H
#define SOLUTION_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <string>
#include <QWebEngineView>
#include <atomic>
#include <thread>

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

    void save_arma_steps();
    void save_arma_steps_async();

    void fetch_async_loop();

    void navigatePrev();
    void navigateNext();
    void updateAnsDisp();
    void setNewSteps(StepsHistory* s);
    void addNewSteps(StepsHistory* s);

    void receiveImage(QPixmap p);
    void on_treeWidget_itemPressed(QTreeWidgetItem *item);

    void closeEvent(QCloseEvent *event) override;

    /**
    * Returns a string that is different from all other string in the variable list.
    */
    string get_usable_steps_string();
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
    * Async variables. Either use atomic or make sure no two variables can be changed with the same thread.
    */
    std::thread *thread_hdl;

    /**
    * Run parser async
    */
    atomic<string*>new_intepret_ptr;
    atomic<string*>new_intepret_err;
    atomic<bool>ascii_or_latex; //true for latex, false for ascii

    atomic<bool>running_parser;
    int parser_last_run_engine;
    int matrix_chosen;

    /**
    * Run lin ops async
    */
    atomic<StepsHistory*>new_step_ptr;
    atomic<bool>running;
    string to_add_steps_name; //The name of the new variable to add in the StepsHistory.
    //We need such a variable sincethe QTreeWidget cannot be accessed in async

    bool running_handled; //If the async tasks are handled by the main thread.

    int translate_token(const TokNum::TokName& t);
signals:
    void finish_sig();
    void next_problem_sig();


public slots:



private slots:
    void openImageFile();
    void computeClicked();
    void saveStepAns();
};

#endif // SOLUTION_WIDGET_H
