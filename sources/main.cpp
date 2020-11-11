/**
 * Temporary main. Includes console for debugging.
 * 
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <gmpxx.h>
#include "math/long/Long.h"
#include "math/long/LongComplex.h"
#include "math/fraction/Fraction.h"
#include "math/polynomial/Polynomial.h"
#include "math/tools.h"
#include "debug_stress_test.h"

using namespace std;

class test{
public:
    mpz_class val;

    test(){
        val="5874395794287504836703472847695376053";
    }

    test(const test& t){
        cout<<"test copy\n";
    }

    test(test&& t){
        cout<<"test move\n";
    }

    ~test(){
        
    }

    virtual test copy();
};

int main(){
    
    run_test();
    return 0;
}
