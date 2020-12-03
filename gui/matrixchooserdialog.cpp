#include "matrixchooserdialog.h"
#include "ui_matrixchooserdialog.h"

#include <QMenu>

MatrixChooserDialog::MatrixChooserDialog(const Info& info, const int& outer_sel, QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint),
    ui(new Ui::MatrixChooserDialog),
    info(info), outer_sel(outer_sel)
{
    ui->setupUi(this);
    matrix_display=new QWebEngineView{ui->scrollArea};
    this->ui->scrollArea->setWidget(matrix_display);

    select_matrix_menu = new QMenu(this);
    for(int i=0;i<info.mat_size.size();i++){
        TokNum::TokName type=info.parsed_mat.at(i).first;

        QAction *menu_action;
        if(i == info.mat_size.size()-1){
            menu_action=new QAction(QString{"Matrix "} + QString::fromStdString(to_string(i)) + (": "+to_display_string(outer_sel)),this);
        }else{
            menu_action=new QAction(QString{"Matrix "} + QString::fromStdString(to_string(i)) + (": "+to_display_string(type)),this);
        }
        select_matrix_menu->addAction(menu_action);
        connect(menu_action,&QAction::triggered,[=](){
            ui->select_matrix->setText(menu_action->text());
            user_select_matrix(i);
        });
    }
    ui->select_matrix->setMenu(select_matrix_menu);

    user_select_matrix(info.mat_size.size()-1);
}

QString MatrixChooserDialog::to_display_string(const TokNum::TokName& t){
    typedef TokNum::TokName T;
    switch(t){
    case T::RREF:
        return "RREF";
    case T::DET:
        return "DET";
    case T::INV:
        return "INVERT";
    case T::SOLVE:
        return "SOLVE";
    case T::CHAR_POLY:
        return "CHAR_POLY";
    case T::ORTH:
        return "ORTHOGONALIZE";
    case T::NA:
        return "NO_OPERATION";
    }
    return "ERROR";
}

QString MatrixChooserDialog::to_display_string(const int &i){
    switch(i){
    case 0:
        return "RREF";
    case 1:
        return "RCEF";
    case 2:
        return "SOLVE";
    case 3:
        return "INVERT";
    case 4:
        return "DET";
    case 5:
        return "CHAR_POLY";
    case 6:
        return "ORTHOGONALIZE";
    }
    return "ERROR";
}

void write_matrix_to_html_latex(ostringstream& os, R** matrix, int rows, int cols); //This is defined in solution_widget.cpp

void MatrixChooserDialog::display(const string& s){
    QString qs="<html><head><script>MathJax = {loader: { load: ['input/asciimath', 'input/tex', 'output/svg', 'ui/menu'] }, tex: {inlineMath: [['$', '$'], ['\\\\(', '\\\\)']]},svg: {fontCache: 'global'}};</script><script type=\"text/javascript\" id=\"MathJax-script\" async src=\"https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-svg.js\"></script><style>div {  margin: auto;  width: auto;  text-align: center;}</style></head><body>\n";
    qs=qs+QString::fromStdString(s);
    qs=qs+"</body></html>";
    matrix_display->setHtml(qs);
    qDebug().noquote();
    qDebug()<<qs;
}

void MatrixChooserDialog::user_select_matrix(int index){
    selected_index=index;
    ui->select_matrix->setText(select_matrix_menu->actions().at(index)->text());

    ostringstream os;
    os<<"$$";
    write_matrix_to_html_latex(os, info.parsed_mat.at(index).second, info.mat_size.at(index).first, info.mat_size.at(index).second);
    os<<"$$\n";
    display(os.str());

    if(index == info.mat_size.size()-1){
        selected_index = -1;
    }
}

MatrixChooserDialog::~MatrixChooserDialog()
{
    delete ui;
}
