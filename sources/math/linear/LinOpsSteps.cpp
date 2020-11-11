#include <iomanip>
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
        delete[]matrix;
    }
    delete[]matrix;

    delete[]console;
    delete[]latex;
}

void LinOpsSteps::print_to_console() const{
    for(int i=0;i<50;i++){
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