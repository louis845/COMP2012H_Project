
/**
 * Returns the GCD of a,b using the Euclidean algorithm.
 * 
*/
template <class T> T gcd(T a, T b){
    if(b>a){
        T temp=a;
        a=b;
        b=temp;
    }
    //The euclidean function of a is larger than or equal to b here.
    T remainder = a % b;
    while(remainder != 0){  //0 is implicitly converted to type T if necessary
        a=b;
        b=remainder;
        remainder=a % b;
    }
    return b;
}