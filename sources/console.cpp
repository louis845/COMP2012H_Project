#include <iostream>
#include <string>
#include <iomanip>

#include "console.h"
#include "math/R.h"
#include "math/long/Long.h"
#include "math/fraction/Fraction.h"
#include "math/linear/LinearOperations.h"
#include "math/tools.h"

using namespace std;

int input_integer(){
    int i;
    cin>>i;
    while(!cin.good()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"Invalid integer! Please enter again: \n";
        cin>>i;
    }
    cin.ignore();
    return i;
}

R input_expression(bool& aborted){
    aborted=false;
    R value;
    string error;
    string input;
    int success;

    while(true){
        cout<<"Please input a mathematical expression. Accepted types are polynomials, complex numbers, fractions, rational polynomials.\n";
        cout<<"To use double precision, type for example 2.0. Do not mix values like '1' (integer) with '1.0' (double) together.\n";
        cout<<"Type quit instead to abort the ENTIRE process of keying in mathematical values.\n";
        getline(cin, input);
        if(input=="quit"){
            aborted=true;
            return value;
        }
        parse_expression(input, error, value, success);
        if(success==-1){ //successful
            cout<<value<<"\n";

            cout<<"type: "<<value.get_type()<<"\n";

            while(true){
                cout<<"Confirm? Type yes/no/quit\n";
                getline(cin,input);
                if(input=="no"){
                    break;
                }else if(input=="yes"){
                    return value;
                }else if(input=="quit"){
                    aborted=true;
                    return value;
                }
            }
        }else{
            for(int i=0;i<success;i++){
                cout<<" ";
            }
            cout<<"^\n";
            cout<<error<<"\n";
        }
    }
    return value;
}

void print_matrix(R** matrix, const int& rows, const int& cols) {
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

/**
 * Allocates and returns a matrix from the console. Does not delete the matrix even if aborted. Ensures that the matrix will be of the same type.
*/
R** input_matrix(const int& rows,const int& cols,bool& aborted){
    aborted=false;
    R** mat=new R*[rows];

    for(int i=0;i<rows;i++){
        mat[i]=new R[cols];
    }
    R largest;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            for(int k=0;k<100;k++){
                cout<<"\n";
            }
            cout<<"Current matrix ( Editing row="+to_string(i+1)+", col="+to_string(j+1)+" ):\n";
            cout<<"type: "<<largest.get_type()<<"\n";
            print_matrix(mat, rows, cols);
            while(true){
                mat[i][j]=input_expression(aborted);
                if(aborted){
                    return mat;
                }
                if(R::complexify_if_needed(largest, mat[i][j])){
                    const NestedRingType& mattype=mat[i][j].get_type();
                    const NestedRingType& largetype=largest.get_type();
                    if(Ring::is_type_subset(mattype,largetype)){
                        largest=mat[i][j];
                    }else{
                        mat[i][j]=largest.promote(mat[i][j]);
                    }
                    break;
                }else{
                    for(int k=0;k<100;k++){
                        cout<<"\n";
                    }
                    cout<<"Type of new element must be compatible with previous elements!\n";
                    cout<<"Current matrix ( Editing row="+to_string(i+1)+", col="+to_string(j+1)+" ):\n";
                    cout<<"type: "<<largest.get_type()<<"\n";
                    print_matrix(mat, rows, cols);
                }
            }
        }
    }

    return mat;
}

void console_main_loop(){
    while(true){
        string s;
        cout<<"type q for quit, else linear operations: ";
        getline(cin, s);

        if(s=="q"){
            return;
        }
        console_linear_operations();
    }
}

void console_linear_operations(){
    cout<<"Input matrix row:";
    int rows=input_integer();
    while(rows<2 || rows>5){
        cout<<"Matrix dimensions must be between 2-5! Please enter again:\n";
        rows=input_integer();
    }

    int cols=input_integer();
    while(cols<2 || cols>5){
        cout<<"Matrix dimensions must be between 2-5! Please enter again:\n";
        cols=input_integer();
    }
    cout<<"Input matrix:\n";
    // use integer for now
    bool aborted=false;
    R** mat_slow=input_matrix(rows,cols,aborted);
    if(aborted){
        for(int i=0;i<rows;i++){
            delete[] mat_slow[i];
        }
        delete[] mat_slow;
        return;
    }
    RF** mat=new RF*[rows];
    for(int i=0;i<rows;i++){
        mat[i]=new RF[cols];
        for(int j=0;j<cols;j++){
            if(!mat_slow[i][j].is_field()){
                R one=mat_slow[i][j].promote_one();
                mat[i][j]=RF{new Fraction{mat_slow[i][j],one}};
            }else{
                mat[i][j]=mat_slow[i][j];
            }
        }
        delete[] mat_slow[i];
    }
    delete[]mat_slow;

    StepsHistory steps;
    LinOpsRecorder rc{&steps, mat, rows, cols};
    LinearOperations o{mat, rows, cols, false, &rc};

    o.toRREF();

    if(steps.has_current_node()){
        while(true){
            steps.get_current_node().print_to_console();
            cout<<"Enter \'next\' to go to next, \'prev\' to go to previous, \'quit\' to end browsing:\n";
            string s;
            getline(cin,s);

            if(s=="next"){
                steps.next_node();
            }else if(s=="prev"){
                steps.previous_node();
            }else if(s=="quit"){
                break;
            }
        }
    }else{
        cout<<"No operations needed!\n";
    }
    for(int i=0;i<rows;i++){
        delete[]mat[i];
    }
    delete[]mat;
    
}