#include <iostream>
#include <string>

#include "console.h"
#include "math/R.h"
#include "math/long/Long.h"
#include "math/fraction/Fraction.h"
#include "math/linear/LinearOperations.h"

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
    return i;
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
        RF** mat=new RF*[rows];

        for(int i=0;i<rows;i++){
            mat[i]=new RF[cols];
            for(int j=0;j<cols;j++){
                int val=input_integer();
                mat[i][j]=RF{new Fraction{RF{new Long{val}},RF{new Long{1}}}};
            }
        }

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
    
}