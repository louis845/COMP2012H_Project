#include "solution_widget.h"
#include "ui_solution_widget.h"
#include "begin_widget.h"
#include <QPainter>
#include <QMessageBox>
#include <QMenu>
#include <fstream>
#include <iostream>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QWebEngineView>
#include <QProgressBar>

#include "math/linear/LinearOperations.h"


solution_widget::solution_widget(string username, string password, QWidget *parent) : QWidget(parent),
    ui(new Ui::solution_widget), parser(""), username(username), password(password)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("CINF");
    this->init_window();
    selected_choice=0;
}

void solution_widget::init_window(){

    void(QTreeWidget:: *tree_item_clicked)(QTreeWidgetItem*,int) = &QTreeWidget::itemClicked;
    void(solution_widget:: *deal_item_pressed)(QTreeWidgetItem*) = &solution_widget::on_treeWidget_itemPressed;

    connect(ui->treeWidget,tree_item_clicked,this,deal_item_pressed);

    ui->disp_prog->setVisible(true);
    ui->disp_prog->setMinimum(0);
    ui->disp_prog->setMaximum(100);
    ui->disp_prog->setValue(0);
    progress_timer = new QTimer;
    connect(progress_timer,&QTimer::timeout,[=](){
        //if (ui->disp_prog != nullptr){
            currentValue++;
            if( currentValue == 100 ) currentValue = 0;
            ui->disp_prog->setValue(currentValue);
            update();
        //}
    });
    progress_timer->start(100);

    //connect(ui->plain_textedit,&QPlainTextEdit::textChanged,this,&solution_widget::handle_plain_update);

    connect(ui->ascii_textedit,&QPlainTextEdit::textChanged,this,&solution_widget::handle_ascii_update);

    connect(ui->scan_btn,&QPushButton::pressed,this,&solution_widget::captureMathExpression);

    connect(ui->previous_btn,&QPushButton::pressed,this,&solution_widget::navigatePrev);

    connect(ui->next_btn,&QPushButton::pressed,this,&solution_widget::navigateNext);

    connect(ui->next_btn2,&QPushButton::pressed,this,&solution_widget::handle_ascii_update);

    ui->scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    scrollarea_layout = new QVBoxLayout(this);

    QMenu * method_menu = new QMenu(this);
    QString list[4]={
        "RREF",
        "RCEF",
        "Solve equation",
        "Inverse matrix"
    };
    for(int i=0;i<4;i++){
        QAction *menu_action=new QAction(list[i],this);
        method_menu->addAction(menu_action);
        connect(menu_action,&QAction::triggered,[=](){
            ui->methods_btn->setText(menu_action->text());
            method_dealer(i);
        });
    }

    ui->methods_btn->setMenu(method_menu);

    connect(ui->finish_btn,&QPushButton::clicked,[=](){emit finish_sig();});

    connect(ui->next_btn,&QPushButton::clicked,[=](){emit next_problem_sig();});

    solution_view=new QWebEngineView{ui->scrollArea};
    ui->scrollArea->setWidget(solution_view);

    intepretation_view=new QWebEngineView{ui->scrollAreaIntepretation};
    ui->scrollAreaIntepretation->setWidget(intepretation_view);

    current_viewing_steps=new StepsHistory;
    current_viewing_steps->add_step(new StepText{"<h1>This is just a test</h1>\n"
                                 "The solutions of the quadratic equation should be displayed if an internet connection is correctly established.<br>\n"
                                 "$$x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a}$$\n"
                                 "<h1>Type something or scan a picture to start</h1>"});
    updateAnsDisp();
    input_window=new begin_widget{username,password};
    input_window->setVisible(false);
    input_window->parent_window=this;
}

void write_matrix_to_html_latex(ostringstream& os, R** matrix, int rows, int cols){
    os<<"\\begin{pmatrix}\n";
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            os<<matrix[i][j].to_latex();
            if(j==cols-1){
                os<<" \\\\\n";
            }else{
                os<<" & ";
            }
        }
    }
    os<<"\\end{pmatrix}\n";
}

void solution_widget::handle_ascii_update(){
    QString text=ui->ascii_textedit->toPlainText();
    string std_text=text.toStdString();
    parser.reset_input(std_text);
    const Info& i=parser.parse();
    qDebug()<<"engine used: "<<i.engine_used;
    qDebug()<<"success: "<<i.success;
    qDebug()<<"intepreted: "<<QString::fromStdString(i.interpreted_input);
    qDebug()<<"mat_size.size(): "<<i.mat_size.size();
    qDebug()<<"parsed_mat.size(): "<<i.parsed_mat.size();
    qDebug()<<QString::fromStdString(i.eval_result);
    qDebug()<<"----------------------------------------------------\n";

    if(i.engine_used==1){
        if(i.mat_size.size()>0){

            const std::pair<int,int> &pr = i.mat_size.at(0);
            R** matrix=i.parsed_mat.at(0).second;
            TokNum::TokName t=i.parsed_mat.at(0).first;
            qDebug()<<static_cast<int>(t);

            ostringstream os;
            os<<"$$";
            const int rows=pr.first;
            const int cols=pr.second;
            write_matrix_to_html_latex(os, matrix,rows,cols);
            os<<"$$";

            display_preview(os.str());
            //question_num++;
            //QTreeWidgetItem* new_question = new QTreeWidgetItem(QStringList(QString::fromStdString("Question"+to_string(question_num)+":")));
            //ui->treeWidget->addTopLevelItem(new_question);

            StepsHistory *steps=nullptr;
            switch(selected_choice){
            case 0:{
                LinearOperationsFunc::row_reduce(matrix, rows, cols, steps);
                break;
            }
            case 1:{
                LinearOperationsFunc::col_reduce(matrix, rows, cols, steps);
                break;
            }
            case 2:{
                LinearOperationsFunc::solve(matrix, rows, cols, steps);
                break;
            }
            case 3:{
                LinearOperationsFunc::invert(matrix, rows, cols, steps);
                break;
            }
            }
            if(steps!=nullptr){
                setNewSteps(steps);
            }
        }
    }else if(i.engine_used==2){
        StepsHistory *steps=new StepsHistory;
        steps->add_step(new StepText{i.eval_result});
        setNewSteps(steps);

        display_preview("`"+i.interpreted_input+"`");
        display_answer(R"(\begin{align*} )" + i.eval_result + R"( \end{align*})");
    }

}

void solution_widget::handle_plain_update(){

}

void solution_widget::captureMathExpression(){
    if(username==""){
        return;
    }

    input_window->show();
    this->setVisible(false);
}

void solution_widget::receiveImage(QPixmap p){
    this->setVisible(true);
    QLabel* original_image;
    original_image->setPixmap(p);
    ui->scrollAreaOriginal->setWidget(original_image);
    std::pair<string, string> result=Ocr::getInstance().request(p);
    string asciimath=result.second;
    ui->ascii_textedit->setPlainText(QString::fromStdString(asciimath));
    handle_ascii_update();

}

void solution_widget::updateAnsDisp(){
    display_answer(current_viewing_steps->get_current_node().get_html_latex());
}

void solution_widget::navigateNext(){
    current_viewing_steps->next_node();
    updateAnsDisp();
}

void solution_widget::navigatePrev(){
    current_viewing_steps->previous_node();
    updateAnsDisp();
}

void solution_widget::setNewSteps(StepsHistory *new_step){

    all_steps_list.push_back(new_step);
    current_viewing_steps=new_step;
    updateAnsDisp();
    //QDateTime current_date_time =QDateTime::currentDateTime();
    //QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    QTime current_time =QTime::currentTime();
    QTreeWidgetItem* ply_item = new QTreeWidgetItem(QStringList("History at "+current_time.toString()));
    ui->treeWidget->addTopLevelItem(ply_item);

    //total_history.
}

void solution_widget::method_dealer(int choice){
    selected_choice=choice;
}

void solution_widget::display_answer(string answer){

    QString qs="<html><head><script>MathJax = {tex: {inlineMath: [['$', '$'], ['\\\\(', '\\\\)']]},svg: {fontCache: 'global'}};</script><script type=\"text/javascript\" id=\"MathJax-script\" async src=\"https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-svg.js\"></script></head><body>\n";
    qs=qs+QString::fromStdString(answer);
    qs=qs+"</body></html>";
    solution_view->setHtml(qs);
    qDebug().noquote();
    qDebug()<<qs;

}

void solution_widget::display_preview(string preview){
    QString qs=R"(<html><head><script>MathJax = {loader: { load: ['input/asciimath', 'input/tex', 'output/svg', 'ui/menu'] }, tex: {inlineMath: [['$', '$'], ['\\(', '\\)']]}, svg: {fontCache: 'global'}, asciimath: {delimiters: [['`','`']]} };</script><script type="text/javascript" id="MathJax-script" async src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/startup.js"></script></head><body>)";
    qs=qs+QString::fromStdString(preview);
    qs=qs+R"(</body></html>)";
    intepretation_view->setHtml(qs);
    qDebug().noquote();
    qDebug()<<qs;
}

void solution_widget::on_treeWidget_itemPressed(QTreeWidgetItem *item){
    //int total_steps_num = all_steps_list.size();
    int chosen_steps = ui->treeWidget->indexOfTopLevelItem(item);
    current_viewing_steps = all_steps_list.at(chosen_steps);
    updateAnsDisp();
}

void solution_widget::paintEvent(QPaintEvent *event)
{
    //Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());
}

solution_widget::~solution_widget()
{
    input_window->close();
    delete ui;
    //delete steps;
    delete progress_timer;
}



