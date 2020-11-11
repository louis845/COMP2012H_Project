#include "tools.h"

R gcd(R a,R b){
    if(b>a){
        R temp=a;
        a=b;
        b=temp;
    }
    if(b.is_zero()){
        return a;
    }
    //The euclidean function of a is larger than or equal to b here.
    R remainder = a % b;
    while(!remainder.is_zero()){
        a=b;
        b=remainder;
        remainder=a % b;
    }
    return b;
}

RF gcd_fast(RF a,RF b){
    if(b>a){
        RF temp=a;
        a=b;
        b=temp;
    }
    if(b.is_zero()){
        return a;
    }
    //The euclidean function of a is larger than or equal to b here.
    RF remainder = a % b;
    while(!remainder.is_zero()){
        a=b;
        b=remainder;
        remainder=a % b;
    }
    return b;
}

long long gcd(long long a,long long b){
    a=abs(a);
    b=abs(b);
    if(b>a){
        long long temp=a;
        a=b;
        b=temp;
    }
    if(b==0){
        return a;
    }
    //The euclidean function of a is larger than or equal to b here.
    long long remainder = a % b;
    while(remainder != 0){
        a=b;
        b=remainder;
        remainder=a % b;
    }
    return b;
}