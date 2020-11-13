#include <iostream>
#include <string>
#include <iomanip>

#include "console.h"
#include "math/R.h"
#include "math/long/Long.h"
#include "math/fraction/Fraction.h"
#include "math/linear/LinearOperations.h"
#include "math/linear/CoupledOperations.h"
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
 * Does not deallocate even if aborted.
*/
R** input_matrix(const int& rows,const int& cols,bool& aborted,const string& disp=""){
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
            cout<<disp<<"\n";
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

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            mat[i][j]=largest.promote(mat[i][j]);
        }
    }

    return mat;
}

void display_steps(StepsHistory& steps){
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
}

void console_linear_operations(bool transpose){
    cout<<"Input matrix row:  ";
    int rows=input_integer();
    while(rows<2 || rows>10){
        cout<<"Matrix dimensions must be between 2-10! Please enter again:\n";
        rows=input_integer();
    }
    cout<<"Input matrix columns:  ";
    int cols=input_integer();
    while(cols<2 || cols>10){
        cout<<"Matrix dimensions must be between 2-10! Please enter again:\n";
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
    LinearOperations o{mat, rows, cols, transpose, &rc};

    o.toRREF();

    for(int i=0;i<rows;i++){
        delete[]mat[i];
    }
    delete[]mat;
    
    display_steps(steps);
}

void console_linear_solve(){
    cout<<"Input number of equations:  ";
    int rows=input_integer();
    while(rows<1 || rows>10){
        cout<<"Number of equations must be between 1-10! Please enter again:\n";
        rows=input_integer();
    }
    cout<<"Input number of variables:  ";
    int cols=input_integer();
    while(cols<1 || cols>10){
        cout<<"Number of variables must be between 2-10! Please enter again:\n";
        cols=input_integer();
    }
    // use integer for now
    bool aborted=false;
    R** mat_slow=input_matrix(rows,cols+1,aborted,"Matrix of linear equations. The last column is the RHS of the linear equations.");
    if(aborted){
        for(int i=0;i<rows;i++){
            delete[] mat_slow[i];
        }
        delete[] mat_slow;
        return;
    }

    RF** mat=new RF*[rows];
    RF** last_col=new RF*[rows];

    for(int i=0;i<rows;i++){
        mat[i]=new RF[cols+1];
        for(int j=0;j<cols+1;j++){
            if(!mat_slow[i][j].is_field()){
                R one=mat_slow[i][j].promote_one();
                mat[i][j]=RF{new Fraction{mat_slow[i][j],one}};
            }else{
                mat[i][j]=mat_slow[i][j];
            }
        }
        delete[] mat_slow[i];

        last_col[i]=(mat[i]+cols); //The submatrix with column starting from col (the last column). No need to do value copy.
    }
    delete[]mat_slow;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols+1;j++){
            cout<<mat[i][j].get_type()<<"\n";
        }
    }

    StepsHistory steps;
    LinOpsRecorder rc{&steps, mat, rows, cols+1};
    CoupledOperations o{mat, rows, cols, false, &rc, last_col, 1};

    o.toRREF();

    for(int i=0;i<rows;i++){
        delete[]mat[i];
    }
    delete[]last_col; //No need to delete last_col since last_col is only stores the pointers that point to the same values as mat.
    delete[]mat;
    
    display_steps(steps);
}

void to_lower_case(string& str){
    transform(str.begin(), str.end(), str.begin(), [](unsigned char ch){ return tolower(ch);});
}

void console_main_loop(){
    while(true){
        string s;
        cout<<"Enter desired computation. Available types are:\n";
        cout<<"SOLVE, REDUCE, INVERT, DETERMINANT, CHAR_POLY\n";
        getline(cin, s);
        to_lower_case(s);
        
        if(s.rfind("solve", 0)==0){
            console_linear_solve();
        }else if(s.rfind("reduce",0)==0){
            int pos=s.find(" ");
            if(pos==string::npos){
                cout<<"Usage: REDUCE <row/col> - Row reduce or column reduce a given matrix.\n";
            }else{
                string arg=s.substr(pos+1,s.length()-pos-1);
                if(arg=="row"){
                    cout<<"Doing row operations. ";
                    console_linear_operations(false);
                }else if(arg=="col"){
                    cout<<"Doing col operations. ";
                    console_linear_operations(true);
                }else{
                    cout<<"Unknown argument. Usage: REDUCE <row/col>\n";
                }
            }
        }
    }
}