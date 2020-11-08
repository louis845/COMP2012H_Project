/**
 * Temporary main. Includes console for debugging.
 * 
*/

#include <iostream>
#include <string>
#include "math/long/Long.h"
#include "math/fraction/Fraction.h"
#include "math/tools.h"

using namespace std;

int main(){
    {
        R a={new Long{-1920}};
        R b={new Long{-1080}};
        R c={new Fraction{a,b}};
        R d={new Fraction{R{new Long{25}},new Long{7}}};

        cout<<a<<"\n";
        cout<<b<<"\n";

        cout<<c<<"\n";
        cout<<d<<"\n";
        cout<<(c+d)<<"\n";
        cout<<(c*d)<<"\n";

        cout<<a<<"\n";
        cout<<b<<"\n";
    }

    int i;
    cin>>i;
    return 0;
}
