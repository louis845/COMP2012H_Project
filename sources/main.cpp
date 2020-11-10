/**
 * Temporary main. Includes console for debugging.
 * 
*/

#include <iostream>
#include <string>
#include <algorithm>
#include "math/long/Long.h"
#include "math/long/LongComplex.h"
#include "math/fraction/Fraction.h"
#include "math/polynomial/Polynomial.h"
#include "math/tools.h"
#include "debug_stress_test.h"

using namespace std;

class test{
public:
    int* val;

    test(){
        val=new int{2};
    }

    ~test(){
        delete val;
    }
};

void test2(const test* const& t){

}

int main(){
    R c1[]={
        R{new Fraction{new Long{-7},new Long{9}}},
        R{new Fraction{new Long{7},new Long{8}}},
        R{new Fraction{new Long{35},new Long{3}}}
    };
    R c2[]={
        R{new Fraction{new Long{14},new Long{1}}},
        R{new Fraction{new Long{14},new Long{5}}},
        R{new Fraction{new Long{1},new Long{1}}}
    };
    R c3[]={
        R{new Fraction{new Long{-7},new Long{3}}},
        R{new Fraction{new Long{56},new Long{3}}},
    };
    R c4[]={
        R{new Fraction{new Long{14},new Long{1}}},
        R{new Fraction{new Long{14},new Long{5}}},
        R{new Fraction{new Long{1},new Long{1}}}
    };
    R f1=new Fraction{new Polynomial{c1,3},new Polynomial{c2,3}};
    R f2=new Fraction{new Polynomial{c3,2},new Polynomial{c4,3}};
    cout<<f1<<"\n";
    cout<<f2<<"\n";

    cout<<f1*f2<<"\n";

    run_test();
    return 0;
}
