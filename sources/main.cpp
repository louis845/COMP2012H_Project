/**
 * Temporary main. Includes console for debugging.
 * 
*/

#include <iostream>
#include <string>
#include "math/double/Double.h"

using namespace std;

int main(){
    string s="test";
    cout<<s<<"\n";

    int i;

    cin>>i;

    R a={new Double{1.4}};
    R b={new Double{2.7}};

    
    cout<<(a/b)<<"\n";

    throw "lol";

    cin>>i;
}
