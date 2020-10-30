/**
 * Temporary main. Includes console for debugging.
 * 
*/

#include <iostream>
#include "math/test.h"

using namespace std;

int main(){
    cout<<"Hello world!"<<endl;
    long a=9;
    long b=12;
    cout<<gcd(a,b)<<endl;
    a=20;
    b=70;
    cout<<gcd(a,b)<<endl;
}