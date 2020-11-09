/**
 * Temporary main. Includes console for debugging.
 * 
*/

#include <iostream>
#include <string>
#include "math/long/Long.h"
#include "math/long/LongComplex.h"
#include "math/fraction/Fraction.h"
#include "math/tools.h"

using namespace std;

int main(){
    {
        R c = {new Fraction{new LongComplex{-4584,54389},new LongComplex{342,54}}};
        R d = {new Fraction{new LongComplex{342,54},new LongComplex{0,-1}}};

        cout<<c<<"\n";
        cout<<d<<"\n\n";
        cout<<(c+d)<<"\n";
        cout<<(c*d)<<"\n";
        cout<<(c/d)<<"\n";
        cout<<(c%d)<<"\n\n";

        cout<<(c/d)<<"\n";
        cout<<(c%d)<<"\n\n";
        
        cout<<(d*(c/d)+(c%d))<<"\n";
    }

    int i;
    cin>>i;
    return 0;
}
