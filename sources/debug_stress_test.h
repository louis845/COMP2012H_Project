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
std::default_random_engine rng;

using namespace std;

void report_error(const R& a, const R& b){
    cout<<a<<"\n";
    cout<<b<<"\n";
    throw "Error!";
}

void test_complex_ops(){
    int divnum=0;
    int strictdivnum=0;
    for(int i=0;i<1000;++i){
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
    for(int i=0;i<1000;++i){
        R a=new Fraction{new Long{values(rng)},new Long{values(rng)}};
        R b=new Fraction{new Long{values(rng)},new Long{values(rng)}};

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
    for(int i=0;i<1000;++i){

        int alen=length(rng);
        int blen=length(rng);
        R* acoeff=new R[alen];
        R* bcoeff=new R[blen];

        for(int j=0;j<alen;j++){
            acoeff[j]=new Fraction{new Long{values(rng)},new Long{values(rng)}};
        }

        for(int j=0;j<blen;j++){
            bcoeff[j]=new Fraction{new Long{values(rng)},new Long{values(rng)}};
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
    for(int i=0;i<1000;++i){
        R a=new Fraction{new LongComplex{values(rng),values(rng)},new Long{values(rng)}};
        R b=new Fraction{new Long{values(rng)},new LongComplex{values(rng),values(rng)}};

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
    for(int i=0;i<1000;++i){

        int alen=length(rng);
        int blen=length(rng);
        R* acoeff=new R[alen];
        R* bcoeff=new R[blen];

        for(int j=0;j<alen;j++){
            acoeff[j]=new Fraction{new Long{values(rng)},new Long{values(rng)}};
        }

        for(int j=0;j<blen;j++){
            bcoeff[j]=new Fraction{new LongComplex{values(rng),values(rng)},new LongComplex{values(rng),values(rng)}};
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

            if(!a.exactly_equals( (a/b)*b + a%b )){
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
    int alen=length(rng);
    int blen=length(rng);
    R* acoeff=new R[alen];
    R* bcoeff=new R[blen];

    for(int j=0;j<alen;j++){
        acoeff[j]=new Fraction{new Long{values(rng)},new Long{values(rng)}};
    }

    for(int j=0;j<blen;j++){
        bcoeff[j]=new Fraction{new Long{values(rng)},new Long{values(rng)}};
    }

    a=new Polynomial{acoeff,alen};
    b=new Polynomial{bcoeff,blen};

    delete[] acoeff;
    delete[] bcoeff;
}

void test_fraction_polynomial_ops(){
    int divnum=0;
    int strictdivnum=0;
    for(int i=0;i<1000;++i){

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

void run_test(){
    while(true){
        cout << "input an integer to choose action:\n";
        cout << "1 - test_complex_ops\n";
        cout << "2 - test_fraction_ops\n";
        cout << "3 - test_polynomial_ops\n";
        cout << "4 - test_complex_fraction_ops\n";
        cout << "5 - test_complex_polynomial_ops\n";
        cout << "6 - test_fraction_polynomial_ops\n";
        cout << "Enter other values to exit:  ";
        int i;
        cin >> i;

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
        default:
            return;
        }
    }
}