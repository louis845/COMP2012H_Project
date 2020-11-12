/**
 * Temporary main. Includes console for debugging.
 * 
*/
#include <gmpxx.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "console.h"
#include "math/R.h"
#include "math/tools.h"


using namespace std;
int main(){
    while(true){
        string input;
        getline(cin,input);
        string err;
        R val;
        int success;

        parse(input, err, val, success);
        
        if(success==-1){
            cout<<val<<"\n";

            cout<<val.get_type()<<"\n";
        }else{
            for(int i=0;i<success;i++){
                cout<<" ";
            }
            cout<<"^\n";
            cout<<err<<"\n";
        }
    }
    mpz_class s;
    s="532584295724895724985723afj430";
    std::cout<<s<<"\n";
    //console_main_loop();
    return 0;
}
