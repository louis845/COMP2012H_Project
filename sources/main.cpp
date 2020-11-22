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
#include <gmpxx.h>

using namespace std;
int main(){
    /*int i;
    cin>>i;
    cin.ignore();
    string s;
    getline(cin,s);
    cout<<s<<"\n";
    cin>>i;*/
    mpz_class a,b;
    a="68249687035694207605346";
    b="42365475386784396783059750349";
    cout<<(b*a)<<"\n";
    console_main_loop();
    return 0;
}
