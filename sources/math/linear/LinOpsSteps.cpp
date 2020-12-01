#include <iomanip>
#include <sstream>
#include "LinOpsSteps.h"

using namespace std;
LinOpsRecorder::LinOpsRecorder(StepsHistory* h, RF** matrix, int rows, int cols) : h(h), matrix(matrix), rows(rows), cols(cols){
    
}

void LinOpsRecorder::capture_instance(string* console, string* latex, int length){
    RF** mat_copy=new RF*[rows];
    for(int i=0;i<rows;i++){
        mat_copy[i]=new RF[cols];
        for(int j=0;j<cols;j++){
            mat_copy[i][j]=matrix[i][j];
        }
    }
    LinOpsSteps* l=new LinOpsSteps{mat_copy,rows,cols,console,latex,length};
    h->add_step(l);
}

void LinOpsRecorder::capture_initial(){
    capture_instance(new string[0],new string[0],0);
}

LinOpsSteps::LinOpsSteps(RF** matrix,int rows,int cols, std::string* console, std::string* latex, int length){
    this->matrix=matrix;
    this->rows=rows;
    this->cols=cols;
    this->console=console;
    this->latex=latex;
    this->length=length;
}

LinOpsSteps::~LinOpsSteps(){
    for(int i=0;i<rows;i++){
        delete[] matrix[i];
    }
    delete[]matrix;

    delete[]console;
    delete[]latex;
}

void LinOpsSteps::print_to_console() const{
    for(int i=0;i<100;i++){
        cout<<"\n";
    }
    cout<<"STEP OPERATIONS: \n";
    for(int i=0;i<length;i++){
        cout<<console[i]<<"\n";
    }
    cout<<"\n";

    int *col_width=new int[cols]; //The string width of each column.
    for(int i=0;i<cols;i++){
        int max=0;
        for(int row=0;row<rows;row++){
            int len=matrix[row][i].to_string().length();
            if(len>max){
                max=len;
            }
        }
        col_width[i]=max+2;
    }

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cout << setw(col_width[j]) << matrix[i][j].to_string();
        }
        cout<<"\n";
    }

    delete[] col_width;
}

void write_matrix_to_html_latex(ostringstream& os, RF** matrix, int rows, int cols){
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

string LinOpsSteps::get_html_latex() const{
    ostringstream os;
    os<<"<h1>Step operations:</h1>\n";
    os<<"\\begin{gather}\n";
    for(int i=0;i<length;i++){
        os<<latex[i]<<"\\\\\n";
    }
    os<<"\\end{gather}\n";
    os<<"<br>";
    os<<"<h1>Matrix after operations:</h1>";
    os<<"\\begin{gather}\n";
    write_matrix_to_html_latex(os,matrix,rows,cols);
    os<<"\\end{gather}\n";
    return os.str();
}

MatrixSpaceStep::MatrixSpaceStep(RF** mat,int rows,int cols,int cutoff,bool row_or_col,const std::string& text) : MatrixSpaceStep(mat,rows,cols,cutoff,row_or_col,text,text){
}

MatrixSpaceStep::MatrixSpaceStep(RF** mat,int rows,int cols,int cutoff,bool row_or_col,const std::string& text, const std::string& latex){
    this->text=text;
    this->latex=latex;
    if(cutoff<0){
        matrix=new RF*[rows];
        space_matrix=nullptr;
        for(int i=0;i<rows;i++){
            matrix[i]=RF::array_copy(mat[i], cols, 0);
        }
        this->rows=rows;
        this->cols=cols;
        this->rows_space=-1;
        this->cols_space=-1;
    }else{
        if(row_or_col){ //row cutoff
            matrix=new RF*[cutoff];
            for(int i=0;i<cutoff;i++){
                matrix[i]=RF::array_copy(mat[i], cols, 0);
            }

            space_matrix=new RF*[rows-cutoff];
            for(int i=cutoff;i<rows;i++){
                space_matrix[i-cutoff]=RF::array_copy(mat[i], cols, 0);
            }
            this->rows=cutoff;
            this->cols=cols;
            this->rows_space=rows-cutoff;
            this->cols_space=cols;
        }else{ //col cutoff
            matrix=new RF*[rows];
            space_matrix=new RF*[rows];
            for(int i=0;i<rows;i++){
                matrix[i]=RF::array_copy(mat[i], cutoff, 0);
                space_matrix[i]=RF::subarray_copy(mat[i],cutoff,cols);
            }

            this->rows=rows;
            this->cols=cutoff;
            this->rows_space=rows;
            this->cols_space=cols-cutoff;
        }
    }
}

MatrixSpaceStep::~MatrixSpaceStep(){
    for(int i=0;i<rows;i++){
        delete[] matrix[i];
    }
    if(space_matrix!=nullptr){
        for(int i=0;i<rows_space;i++){
            delete[] space_matrix[i];
        }
    }

    delete[]space_matrix;
    delete[]matrix;
}

void enclosed_matrix_print(RF** matrix, const int& rows, const int& cols){
    int *col_width=new int[cols]; //The string width of each column.
    for(int i=0;i<cols;i++){
        int max=0;
        for(int row=0;row<rows;row++){
            int len=matrix[row][i].to_string().length();
            if(len>max){
                max=len;
            }
        }
        col_width[i]=max+2;
    }

    for(int i=0;i<rows;i++){
        cout<<" [";
        for(int j=0;j<cols;j++){
            cout << setw(col_width[j]) << matrix[i][j].to_string();
        }
        cout<<" ]\n";
    }

    delete[] col_width;
}

void MatrixSpaceStep::print_to_console() const{
    for(int i=0;i<100;i++){
        cout<<"\n";
    }
    cout<<text<<"\n";
    cout<<"\n";
    if(space_matrix!=nullptr){
        cout<<"The matrix A is:\n";
        enclosed_matrix_print(matrix,rows,cols);
        cout<<"The matrix B is:\n";
        enclosed_matrix_print(space_matrix,rows_space,cols_space);
    }else{
        enclosed_matrix_print(matrix,rows,cols);
    }
}

string MatrixSpaceStep::get_html_latex() const{
    ostringstream os;
    os<<""<<latex<<"\n";
    cout<<"<br>\n";
    if(space_matrix!=nullptr){
        os<<"\\begin{gather}\n";
        os<<"A=";
        write_matrix_to_html_latex(os,this->matrix,rows,cols);
        os<<"\\\\\nB=";
        write_matrix_to_html_latex(os,space_matrix,rows_space,cols_space);
        os<<"\\end{gather}\n";
    }else{
        os<<"\\begin{gather}\n";
        write_matrix_to_html_latex(os,this->matrix,rows,cols);
        os<<"\\end{gather}\n";
    }
    return os.str();
}

RF** MatrixSpaceStep::get_matrix_copy(){
    return RF::copy_matrix(matrix,rows,cols);
}

const int& MatrixSpaceStep::get_matrix_rows(){
    return rows;
}

const int& MatrixSpaceStep::get_matrix_cols(){
    return cols;
}