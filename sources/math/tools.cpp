#include "tools.h"
#include <iostream>
R gcd(R a,R b){
    if(b>a){
        R temp=a;
        a=b;
        b=temp;
    }
    //The euclidean function of a is larger than or equal to b here.
    R remainder = a % b;
    while(!remainder.is_zero()){  //0 is implicitly converted to type T if necessary
        a=b;
        b=remainder;
        remainder=a % b;
    }
    return b;
}