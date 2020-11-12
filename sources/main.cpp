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
#include "math/long/LongComplex.h"
#include "math/fraction/Fraction.h"
#include "math/polynomial/Polynomial.h"


using namespace std;
int main(){
    R coeff[2]{
        new Fraction{new Long{1},new Long{1}},
        new Fraction{new Long{2},new Long{3}},
    };
    R py=new Polynomial{coeff, 2};
    R val=new LongComplex{1,0};
    cout<<py.get_type()<<"\n";
    cout<<py.complexify().get_type()<<"\n";
    cout<<val.get_type()<<"\n";
    cout<<py.is_type_compatible(val)<<"\n";
    cout<<py.complexify().is_type_compatible(val)<<"\n";
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
    //console_main_loop();
    return 0;
}
