#include "solution_widget.h"
#include "ui_solution_widget.h"
#include "begin_widget.h"
#include <QPainter>
#include <QMessageBox>
#include <QMenu>
#include <fstream>
#include <iostream>
#include <thread>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QProgressBar>
#include <algorithm>

#include "math/linear/LinearOperations.h"


solution_widget::solution_widget(string username, string password, QWidget *parent) : QWidget(parent),
    ui(new Ui::solution_widget), parser(""), username(username), password(password)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("CINF");
    this->init_window();
    selected_choice=0;

    running=false;
    running_parser=false;
    running_handled=true;
    new_step_ptr=nullptr;
    new_intepret_ptr=nullptr;
    new_intepret_err=nullptr;
    ascii_or_latex=true;
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
    connect(progress_timer,&QTimer::timeout,this,&solution_widget::fetch_async_loop);

    connect(ui->ascii_textedit,&QPlainTextEdit::textChanged,this,&solution_widget::handle_ascii_update);

    connect(ui->scan_btn,&QPushButton::pressed,this,&solution_widget::captureMathExpression);

    connect(ui->previous_btn,&QPushButton::pressed,this,&solution_widget::navigatePrev);

    connect(ui->next_btn,&QPushButton::pressed,this,&solution_widget::navigateNext);

    connect(ui->next_btn2,&QPushButton::pressed,this,&solution_widget::run_solver);

    ui->scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    scrollarea_layout = new QVBoxLayout(this);

    QMenu * method_menu = new QMenu(this);
    QString list[7]={
        "RREF",
        "RCEF",
        "Solve equation",
        "Inverse matrix",
        "Determinant",
        "Characteristc polynomial",
        "Orthogonalization"
    };
    for(int i=0;i<7;i++){
        QAction *menu_action=new QAction(list[i],this);
        method_menu->addAction(menu_action);
        connect(menu_action,&QAction::triggered,[=](){
            ui->methods_btn->setText(menu_action->text());
            method_dealer(i);
        });
    }

    ui->methods_btn->setMenu(method_menu);

    connect(ui->jump_btn,&QPushButton::clicked,[=](){
        if (current_viewing_steps!=nullptr){
            current_viewing_steps->last_node();
            updateAnsDisp();
        }
    });

    connect(ui->next_btn,&QPushButton::clicked,[=](){emit next_problem_sig();});

    solution_view=new QWebEngineView{ui->scrollArea};
    solution_view->settings()->setAttribute(QWebEngineSettings::FocusOnNavigationEnabled, false);
    ui->scrollArea->setWidget(solution_view);

    intepretation_view=new QWebEngineView{ui->scrollAreaIntepretation};
    intepretation_view->settings()->setAttribute(QWebEngineSettings::FocusOnNavigationEnabled, false);
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

/**
* Helper function to write matrix to ostringstream
*/
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
    if(running_handled){
        running_handled=false;
        running_parser=true;
        progress_timer->start(20);

        QString text=ui->ascii_textedit->toPlainText();
        std::string std_text=text.toStdString();
        if(std_text.rfind('=')==string::npos){
            std_text.erase(std::remove(std_text.begin(), std_text.end(), '\n'), std_text.end());
            std_text.erase(std::remove(std_text.begin(), std_text.end(), '\r'), std_text.end());
        }else{
            std_text=regex_replace(std_text,regex("\\n"),"\\\\");
        }
        std::thread thrd(&solution_widget::handle_ascii_update_async, this, std_text);
        thrd.detach();
    }
}

void solution_widget::handle_ascii_update_async(string text){
    parser.reset_input(text);
    const Info& i=parser.parse();
    /*qDebug()<<"engine used: "<<i.engine_used;
    qDebug()<<"success: "<<i.success;
    qDebug()<<"intepreted: "<<QString::fromStdString(i.interpreted_input);
    qDebug()<<"mat_size.size(): "<<i.mat_size.size();
    qDebug()<<"parsed_mat.size(): "<<i.parsed_mat.size();
    qDebug()<<QString::fromStdString(i.eval_result);
    qDebug()<<"----------------------------------------------------\n";*/
    //probably cannot use Q functions in async

    string* ns_addr=nullptr;
    string* err_addr=nullptr;
    if(i.success){
        if(i.engine_used==1 || i.engine_used==3){

            ostringstream os;
            os << "<div>`" << i.interpreted_input << "`</div>";
            os<<R"( \begin{align*} = )";
            // const int rows=pr.first;
            // const int cols=pr.second;
            // write_matrix_to_html_latex(os, matrix,rows,cols); //just helper function to write to ostringstream
            os << i.eval_result;
            os<<R"( \end{align*} )";
            ns_addr=new string{os.str()};
            ascii_or_latex=true;
            err_addr=new string{"No error!"};

            if(i.mat_size.size()>0){
                const int last_elem=i.mat_size.size()-1;
                const std::pair<int,int> &pr = i.mat_size.at(last_elem);
                R** matrix=i.parsed_mat.at(last_elem).second;
                TokNum::TokName t=i.parsed_mat.at(last_elem).first;
                qDebug()<<static_cast<int>(t);
            }else{
                err_addr=new string{i.err_msg};
            }
        }else{
            string to_disp="`"+i.interpreted_input+"`";
            ns_addr=new string{to_disp};
            ascii_or_latex=false;
            err_addr=new string{"No error!"};
        }
    }else{
        err_addr=new string{i.err_msg};
    }
    new_intepret_ptr=ns_addr;
    new_intepret_err=err_addr;
    running_parser=false;
}

void solution_widget::run_solver(){
    if(running_handled){
        running_handled=false;
        running=true;
        progress_timer->start(20);

        std::thread thrd{&solution_widget::run_solver_async, this};
        thrd.detach();
    }
}

void solution_widget::run_solver_async(){
    const Info& i=parser.getInfo();
    StepsHistory *steps=nullptr;
    if(i.success){
        if(i.engine_used==1){
            if(i.mat_size.size()>0){
                const int last_elem=i.mat_size.size()-1;
                const std::pair<int,int> &pr = i.mat_size.at(last_elem);
                R** matrix=i.parsed_mat.at(last_elem).second;
                TokNum::TokName t=i.parsed_mat.at(last_elem).first;

                const int rows=pr.first;
                const int cols=pr.second;

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
                case 4:{
                    LinearOperationsFunc::determinant(matrix, rows, cols, steps);
                    break;
                }
                case 5:{
                    LinearOperationsFunc::char_poly(matrix, rows, cols, steps);
                    break;
                }
                case 6:{
                    LinearOperationsFunc::orthogonalize(matrix, rows, cols, steps);
                    break;
                }
                }
            }
        }else if(i.engine_used==2){
            steps=new StepsHistory;
            steps->add_step(new StepText{R"(\begin{align*} )" + i.eval_result + R"( \end{align*})"});
        }
    }

    new_step_ptr=steps;
    running=false;
}

void solution_widget::fetch_async_loop(){
    if((!running) && (!running_parser)){
        string *s=new_intepret_ptr;
        string *err=new_intepret_err;
        StepsHistory *hist=new_step_ptr;

        new_intepret_ptr=nullptr;
        new_intepret_err=nullptr;
        new_step_ptr=nullptr;

        if(s!=nullptr){
            if(ascii_or_latex){
                display_preview(*s);
            }else{
                display_preview_ascii(*s);
            }
            delete s;
        }
        if(err!=nullptr){
            ui->ascii_parser_err->setText(QString::fromStdString(*err));
            delete err;
        }
        if(hist!=nullptr){
            setNewSteps(hist);
        }
        progress_timer->stop();
        running_handled=true;

        currentValue=0;
        ui->disp_prog->setValue(currentValue);
        ui->disp_prog->update();
    }else{
        currentValue = (currentValue+1)%100;
        ui->disp_prog->setValue(currentValue);
        ui->disp_prog->update();
    }
}

void solution_widget::captureMathExpression(){
    if(username==""){
        QMessageBox::information(this,"Error","OCR is not enabled.",QMessageBox::Ok);
        return;
    }
    if(!running_handled){
        QMessageBox::information(this,"Error","Sorry, cannot do OCR with ongoing computation.",QMessageBox::Ok);
    }

    input_window->show();
    this->setVisible(false);
}

void solution_widget::receiveImage(QPixmap p){
    this->setVisible(true);
    ui->original_image_display->setPixmap(p);
    ui->original_image_display->resize(p.width(),p.height());
    dynamic_cast<QWidget*>(ui->original_image_display->parent())->resize(p.width(),p.height());

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
    ui->treeWidget->setCurrentItem(ply_item);
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
    QString qs="<html><head><script>MathJax = {loader: { load: ['input/asciimath', 'input/tex', 'output/svg', 'ui/menu'] }, tex: {inlineMath: [['$', '$'], ['\\\\(', '\\\\)']]},svg: {fontCache: 'global'}};</script><script type=\"text/javascript\" id=\"MathJax-script\" async src=\"https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-svg.js\"></script><style>div {  margin: auto;  width: auto;  text-align: center;}</style></head><body>\n";
    qs=qs+QString::fromStdString(preview);
    qs=qs+"</body></html>";
    intepretation_view->setHtml(qs);
    qDebug().noquote();
    qDebug()<<qs;
}

void solution_widget::display_preview_ascii(string preview){
    display_preview(preview);
    /*QString qs="<html><head><script>MathJax = {  loader: {load: ['input/asciimath', 'input/tex', 'output/chtml', 'ui/menu']},  asciimath: {    delimiters: [['$','$'],['`','`']]  },};</script><style>div {  margin: auto;  width: auto;  text-align: center;}</style><script type=\"text/javascript\" id=\"MathJax-script\" async  src=\"https://cdn.jsdelivr.net/npm/mathjax@3/es5/startup.js\"></script></head><body>\n";
    qs=qs+"<div>";
    qs=qs+QString::fromStdString(preview);
    qs=qs+"</div></body></html>";
    intepretation_view->setHtml(qs);
    qDebug().noquote();
    qDebug()<<qs;*/
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
    delete progress_timer;

    for(int i=0;i<all_steps_list.size();++i){
        delete all_steps_list.at(i);
    }
}

void solution_widget::closeEvent(QCloseEvent *event){
    if(!running_handled){
        event->ignore();
        QMessageBox::information(this,"Error","Sorry, cannot close window with ongoing computation!",QMessageBox::Ok);
    }
}


