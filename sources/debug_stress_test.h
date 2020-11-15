/**
 * Header file for stress test debug functions. ONLY include this file in the main.cpp
*/

#include <random>
#include <iostream>
#include "math/long/LongComplex.h"
#include "math/long/Long.h"
#include "math/fraction/Fraction.h"
#include "math/polynomial/Polynomial.h"

std::uniform_int_distribution<int> values(-10,10);
std::uniform_int_distribution<int> length(1,4);
std::uniform_int_distribution<int> small(1,3);
std::default_random_engine rng;

using namespace std;

void report_error(const R& a, const R& b){
    cout<<a<<"\n";
    cout<<b<<"\n";
    throw "Error!";
}

int non_zero_value(){
    int val=values(rng);

    if(val==0){
        val=1;
    }
    return val;
}

void non_zero_value(int &a,int &b){
    a=values(rng);
    b=values(rng);

    if(a==0 && b==0){
        a=1;
    }
}

void test_complex_ops(){
    int divnum=0;
    int strictdivnum=0;
    for(int i=0;i<500;++i){
        R a=new LongComplex{values(rng),values(rng)};
        R b=new LongComplex{values(rng),values(rng)};

        R c=a+b;
        R d=a*b;
        if(!c.exactly_equals(a+b)){
            report_error(a,b);
        }

        if(!d.exactly_equals(a*b)){
            report_error(a,b);
        }

        if(!b.is_zero()){
            divnum++;

            R e=a/b;
            R f=a%b;

            if(!e.exactly_equals(a/b)){
                report_error(a,b);
            }

            if(!f.exactly_equals(a%b)){
                report_error(a,b);
            }

            if(!a.exactly_equals( (a/b)*b + a%b )){
                report_error(a,b);
            }

            if(!e.is_zero()){
                strictdivnum++;
            }
        }
    }
    cout << "complex ops test: "<<divnum<<" "<<strictdivnum<<"\n";
}

void test_fraction_ops(){
    int divnum=0;
    int strictdivnum=0;
    for(int i=0;i<500;++i){
        R a=new Fraction{new Long{values(rng)},new Long{non_zero_value()}};
        R b=new Fraction{new Long{values(rng)},new Long{non_zero_value()}};

        R c=a+b;
        R d=a*b;
        if(!c.exactly_equals(a+b)){
            report_error(a,b);
        }

        if(!d.exactly_equals(a*b)){
            report_error(a,b);
        }

        if(!b.is_zero()){
            divnum++;

            R e=a/b;
            R f=a%b;

            if(!e.exactly_equals(a/b)){
                report_error(a,b);
            }

            if(!f.exactly_equals(a%b)){
                report_error(a,b);
            }

            if(!a.exactly_equals( (a/b)*b + a%b )){
                report_error(a,b);
            }

            if(!e.is_zero()){
                strictdivnum++;
            }
        }
    }
    cout << "fraction ops test: "<<divnum<<" "<<strictdivnum<<"\n";
}

void test_polynomial_ops(){
    int divnum=0;
    int strictdivnum=0;
    for(int i=0;i<500;++i){

        int alen=length(rng);
        int blen=length(rng);
        R* acoeff=new R[alen];
        R* bcoeff=new R[blen];

        for(int j=0;j<alen;j++){
            acoeff[j]=new Fraction{new Long{values(rng)},new Long{non_zero_value()}};
        }

        for(int j=0;j<blen;j++){
            bcoeff[j]=new Fraction{new Long{values(rng)},new Long{non_zero_value()}};
        }

        R a=new Polynomial{acoeff,alen};
        R b=new Polynomial{bcoeff,blen};

        R c=a+b;
        R d=a*b;
        if(!c.exactly_equals(a+b)){
            report_error(a,b);
        }

        if(!d.exactly_equals(a*b)){
            report_error(a,b);
        }

        if(!b.is_zero()){
            divnum++;
            R e=a/b;

            R f=a%b;
            
            if(!e.exactly_equals(a/b)){
                report_error(a,b);
            }

            if(!f.exactly_equals(a%b)){
                report_error(a,b);
            }

            if(!a.exactly_equals( (a/b)*b + a%b )){
                report_error(a,b);
            }

            if(!e.is_zero()){
                strictdivnum++;
            }
        }
    }
    cout << "polynomial ops test: "<<divnum<<" "<<strictdivnum<<"\n";
}




void test_complex_fraction_ops(){
    int divnum=0;
    int strictdivnum=0;
    for(int i=0;i<500;++i){
        R a=new Fraction{new LongComplex{values(rng),values(rng)},new Long{non_zero_value()}};
        int val1,val2;
        non_zero_value(val1,val2);
        R b=new Fraction{new Long{values(rng)},new LongComplex{val1,val2}};

        R c=a+b;
        R d=a*b;
        if(!c.exactly_equals(a+b)){
            report_error(a,b);
        }

        if(!d.exactly_equals(a*b)){
            report_error(a,b);
        }

        if(!b.is_zero()){
            divnum++;

            R e=a/b;
            R f=a%b;

            if(!e.exactly_equals(a/b)){
                report_error(a,b);
            }

            if(!f.exactly_equals(a%b)){
                report_error(a,b);
            }

            if(!a.exactly_equals( (a/b)*b + a%b )){
                report_error(a,b);
            }

            if(!e.is_zero()){
                strictdivnum++;
            }
        }
    }
    cout << "complex fraction ops test: "<<divnum<<" "<<strictdivnum<<"\n";
}

void test_complex_polynomial_ops(){
    int divnum=0;
    int strictdivnum=0;
    for(int i=0;i<500;++i){
        
        int alen=length(rng);
        int blen=length(rng);
        R* acoeff=new R[alen];
        R* bcoeff=new R[blen];

        for(int j=0;j<alen;j++){
            acoeff[j]=new Fraction{new Long{values(rng)},new Long{non_zero_value()}};
        }

        for(int j=0;j<blen;j++){
            int val1,val2;
            non_zero_value(val1,val2);
            bcoeff[j]=new Fraction{new LongComplex{values(rng),values(rng)},new LongComplex{val1,val2}};
        }

        R a=new Polynomial{acoeff,alen};
        R b=new Polynomial{bcoeff,blen};

        delete[] acoeff;
        delete[] bcoeff;

        R c=a+b;
        R d=a*b;
        
        if(!c.exactly_equals(a+b)){
            report_error(a,b);
        }

        if(!d.exactly_equals(a*b)){
            report_error(a,b);
        }

        if(!b.is_zero()){
            divnum++;
            R e=a/b;
            R f=a%b;

            if(!e.exactly_equals(a/b)){
                report_error(a,b);
            }

            if(!f.exactly_equals(a%b)){
                report_error(a,b);
            }
            R expr=(a/b)*b + a%b;

            if(!a.exactly_equals(expr)){
                report_error(a,b);
            }

            if(!e.is_zero()){
                strictdivnum++;
            }
        }
    }
    cout << "complex polynomial ops test: "<<divnum<<" "<<strictdivnum<<"\n";
}

void gen_poly(R& a, R& b){
    int alen=small(rng);
    int blen=small(rng);
    R* acoeff=new R[alen];

    for(int j=0;j<alen;j++){
        acoeff[j]=new Fraction{new Long{values(rng)},new Long{non_zero_value()}};
    }

    a=new Polynomial{acoeff,alen};
    delete[] acoeff;

    while(b.is_zero()){

        R* bcoeff=new R[blen];

        for(int j=0;j<blen;j++){
            bcoeff[j]=new Fraction{new Long{values(rng)},new Long{non_zero_value()}};
        }

        b=new Polynomial{bcoeff,blen};
        delete[] bcoeff;
    }
}

void test_fraction_polynomial_ops(){
    int divnum=0;
    int strictdivnum=0;
    for(int i=0;i<500;++i){

        R py1,py2;

        gen_poly(py1,py2);

        R a=new Fraction{py1,py2};

        gen_poly(py1,py2);

        R b=new Fraction{py1,py2};

        R c=a+b;
        R d=a*b;
        if(!c.exactly_equals(a+b)){
            report_error(a,b);
        }

        if(!d.exactly_equals(a*b)){
            report_error(a,b);
        }

        if(!c.exactly_equals(a+b)){
            report_error(a,b);
        }

        if(!d.exactly_equals(a*b)){
            report_error(a,b);
        }

        if(!b.is_zero()){
            divnum++;

            R e=a/b;
            R f=a%b;

            if(!e.exactly_equals(a/b)){
                report_error(a,b);
            }

            if(!f.exactly_equals(a%b)){
                report_error(a,b);
            }

            if(!a.exactly_equals( (a/b)*b + a%b )){
                report_error(a,b);
            }

            if(!e.is_zero()){
                strictdivnum++;
            }
        }
    }
    cout << "fraction polynomial ops test: "<<divnum<<" "<<strictdivnum<<"\n";
}

void gen_poly_complex(R& a, R& b){
    int alen=small(rng);
    int blen=small(rng);
    R* acoeff=new R[alen];

    for(int j=0;j<alen;j++){
        acoeff[j]=new Fraction{new Long{values(rng)},new Long{non_zero_value()}};
    }

    a=new Polynomial{acoeff,alen};
    delete[] acoeff;

    while(b.is_zero()){

        R* bcoeff=new R[blen];

        for(int j=0;j<blen;j++){
            bcoeff[j]=new Fraction{new LongComplex{values(rng),values(rng)},new Long{non_zero_value()}};
        }

        b=new Polynomial{bcoeff,blen};
        delete[] bcoeff;
    }
}

void test_fraction_complex_polynomial_ops(){
    int divnum=0;
    int strictdivnum=0;
    for(int i=0;i<500;++i){

        R py1,py2;

        gen_poly(py1,py2);

        R a=new Fraction{py1,py2};

        gen_poly_complex(py1,py2);

        R b=new Fraction{py1,py2};

        R c=a+b;
        R d=a*b;
        if(!c.exactly_equals(a+b)){
            report_error(a,b);
        }

        if(!d.exactly_equals(a*b)){
            report_error(a,b);
        }

        if(!c.exactly_equals(a+b)){
            report_error(a,b);
        }

        if(!d.exactly_equals(a*b)){
            report_error(a,b);
        }

        if(!b.is_zero()){
            divnum++;

            R e=a/b;
            R f=a%b;

            if(!e.exactly_equals(a/b)){
                report_error(a,b);
            }

            if(!f.exactly_equals(a%b)){
                report_error(a,b);
            }

            if(!a.exactly_equals( (a/b)*b + a%b )){
                report_error(a,b);
            }

            if(!e.is_zero()){
                strictdivnum++;
            }
        }
    }
    cout << "fraction polynomial ops test: "<<divnum<<" "<<strictdivnum<<"\n";
}

void run_test(){
    while(true){
        cout << "input an integer to choose action:\n";
        cout << "1 - test_complex_ops\n";
        cout << "2 - test_fraction_ops\n";
        cout << "3 - test_polynomial_ops\n";
        cout << "4 - test_complex_fraction_ops\n";
        cout << "5 - test_complex_polynomial_ops\n";
        cout << "6 - test_fraction_polynomial_ops\n";
        cout << "7 - test_fraction_complex_polynomial_ops\n";
        cout << "Enter other values to exit:  ";
        int i;
        i=7;
        

        switch(i){
        case 1:
            test_complex_ops();
            break;
        case 2:
            test_fraction_ops();
            break;
        case 3:
            test_polynomial_ops();
            break;
        case 4:
            test_complex_fraction_ops();
            break;
        case 5:
            test_complex_polynomial_ops();
            break;
        case 6:
            test_fraction_polynomial_ops();
            break;
        case 7:
            test_fraction_complex_polynomial_ops();
            break;
        default:
            return;
        }
    }
}