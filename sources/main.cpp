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
    {
        R coeff1[3]={
            new Fraction{new LongComplex{5,2},new LongComplex{-4,7}},
            new Fraction{new LongComplex{4,3},new LongComplex{2,5}},
            new Fraction{new LongComplex{8,-7},new LongComplex{9,-2}}
        };

        R coeff2[4]={
            new Fraction{new LongComplex{2,2},new Long{1}},
            new Fraction{new LongComplex{5,3},new Long{1}},
            new Fraction{new LongComplex{7,2},new Long{1}},
            new Fraction{new LongComplex{9,0},new Long{1}}
        };

        R c=new Polynomial{coeff1,3};
        R d=new Polynomial{coeff2,4};

        cout<<c<<"\n";
        cout<<d<<"\n\n";

        R e=c/d;
        d*e;
        cout<<c<<"\n";
        cout<<d<<"\n\n";
        cout<<(c+d)<<"\n";
        cout<<(c*d)<<"\n";
        cout<<(c/d)<<"\n";
        cout<<(c%d)<<"\n\n";

        cout<<(c/d)<<"\n";
        cout<<(c%d)<<"\n\n";
        
        cout<<c-(d*(c/d)+c%d)<<"\n";
        cout<<c<<"\n";
        cout<<d<<"\n\n";
    }

    int ii;
    std::cin>>ii;
    return 0;
}
