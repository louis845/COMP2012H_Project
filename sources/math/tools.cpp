#include "tools.h"
#include "math/polynomial/Polynomial.h"
#include "math/fraction/Fraction.h"
#include "math/long/LongComplex.h"
#include "math/double/DoubleComplex.h"
#include "math/double/Double.h"

using namespace std;

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

MathFormulaTree::MathFormulaTree(string formula,char oper){
    this->formula=formula;
    left=right=nullptr;
    this->oper=oper;
}

MathFormulaTree::~MathFormulaTree(){
    delete left;
    delete right;
}

/**
 * Ensures the input is of allowed characters. 
 * poly_char - the character of the polynomial, -1 if none found
*/
void ensure_basic_char(const string& input, char& poly_char, string& err, int& success){
    if(input.length()==0){
        success=0;
        err="Empty string!";
        return;
    }

    int bracket_depth=0;
    poly_char=-1;
    for(int i=0;i<input.length();i++){
        const char& c=input.at(i);
        if('0'<=c && c<='9'){
            //numbers
        }else if(c=='i'){
            //complex
        }else if( ('A'<=c && c<='Z') || ('a'<=c && c<='z') ){
            //poly char
            if(poly_char!=-1 && c!=poly_char){
                success=i;
                err="Polynomial of two or more variables is not supported!";
                return;
            }else{
                poly_char=c;
            }
        }else if(c=='+' || c=='-' || c=='*' || c=='/' || c=='.'){

        }else if(c=='('){
            bracket_depth++;
        }else if(c==')'){
            if(bracket_depth>0){
                bracket_depth--;
            }else{
                err="Nothing to close bracket here!";
                success=i;
                return;
            }
        }else{
            success=i;
            err="Unknown character";
            return;
        }
    }
    if(bracket_depth!=0){
        err="Unclosed brackets";
        success=input.length()-1;
    }
}

/**
 * Returns true iff c1>c2, strictly, without considering unary.
*/
bool reverse_precedence(const char& c1,const char& c2){
    return (c2!=-1 && ((c1=='*' || c1=='/' || c1== '(' || c1==')') && (c2=='+' || c2=='-')));
}

void simple_parse_cpx(const string& input, string& err, R& val, int& success, const int& start_index, const int& end_index){
    if(start_index>=end_index){
        err="Expected expression in here";
        success=start_index;
        return;
    }
    string substr=input.substr(start_index, end_index-start_index);
    
    if(substr=="i"){
        val=R{new LongComplex{0,1}};
        return;
    }
    if(substr.find('.')!=string::npos){
        string pdouble;
        bool complex=false;
        if(input.at(end_index-1)=='i'){
            pdouble=input.substr(start_index, end_index-1-start_index);
            complex=true;
        }else{
            pdouble=input.substr(start_index, end_index-start_index);
        }
        double d;
        try{
            d=stod(pdouble);
        }catch(...){
            err="Double detected, but invalid double: "+pdouble;
            success=start_index;
            return;
        }
        if(complex){
            val=R{ new DoubleComplex{0.0,d}}; //TODO, double complex not yet impl!
        }else{
            val=R{ new Double{d}};
        }
        return;
    }else{
        string plong;
        bool complex=false;
        if(input.at(end_index-1)=='i'){
            plong=input.substr(start_index, end_index-1-start_index);
            complex=true;
        }else{
            plong=input.substr(start_index, end_index-start_index);
        }
        mpz_wrapper l(0);
        try{
            l=plong;
        }catch(...){
            err="Long detected, but invalid long: "+plong;
            success=start_index;
            return;
        }
        if(complex){
            val=R{ new LongComplex{0,l}};
        }else{
            val=R{ new Long{l}};
        }
        return;
    }
}

/** end index is not inclusive
 * Operator precedence: + and - (unary) > * / > + -
*/
void parse_recurse(const string& input, string& err, R& val, int& success, const int& start_index, const int& end_index, const char& poly_char){
    if(start_index>=end_index){
        throw "debug";
        err="Expected expression in here";
        success=start_index;
        return;
    }
    bool unary=true; //Is current char unary?
    int cutoff=start_index;
    int unary_cutoff=start_index;

    bool negate=false; //Unary needs negate? (Number of minuses)

    int bracket_depth=0;

    char op=-1;
    for(int i=start_index;i<end_index;i++){
        const char& c=input.at(i);
        if(bracket_depth==0){
            bool bracket_mult=true;
            if(unary){
                if(c=='*'||c=='/'){
                    success=i;
                    err="Does not expect * or /";
                    return;
                }else if(c=='-'){
                    negate= !negate;
                }else if(c=='+'){

                }else{
                    unary_cutoff=i;
                    unary=false;
                }
                bracket_mult=false;
            }
            if(!unary){
                if(c=='+' || c=='-' || c== '*' || c=='/'){
                    if(!reverse_precedence(c,op)){
                        //here c <= op
                        cutoff=i;
                        op=c;
                    }
                    unary=true;
                }else if(c=='('){ //implicit multiplication, both cases ')(' or '<some other val>(' are considered
                    if(bracket_mult && !reverse_precedence('*',op)){
                        //here c <= op
                        cutoff=i;
                        op=c;
                    }
                    ++bracket_depth;
                }else if(i>start_index && input.at(i-1)==')'){
                    //implicit multiplication
                    if(!reverse_precedence('*',op)){
                        //here c <= op
                        cutoff=i;
                        op=')';
                    }
                }
            }
        }else{
            if(c=='('){
                ++bracket_depth;
            }else if(c==')'){
                --bracket_depth;
                unary=false;
            }
        }
    }
    
    if(op!=-1){
        R front;
        R back;
        parse_recurse(input, err, front, success, start_index, cutoff, poly_char);
        if(success!=-1){
            return;
        }
        if(op=='('){
            //bracket, remove the ending bracket ')'
            parse_recurse(input, err, back, success, cutoff+1, end_index-1, poly_char);
        }else if(cutoff>start_index && op==')'){
            //implicitly multiply with ')'
            parse_recurse(input, err, back, success, cutoff, end_index,poly_char);
        }else{
            // + - * / , no need to include
            parse_recurse(input, err, back, success, cutoff+1, end_index, poly_char);
        }
        
        if(success!=-1){
            return;
        }

        if(!front.is_type_compatible(back)){
            if(front.get_type().complex() && !back.get_type().complex()){
                R cpx=back.complexify();
                if(!front.is_type_compatible(cpx)){
                    success=cutoff;
                    err="Cannot do operation on non compatible types!";
                    return;
                }
                back=cpx;
            }else if(!front.get_type().complex() && back.get_type().complex()){
                R cpx=front.complexify();
                if(!cpx.is_type_compatible(back)){
                    success=cutoff;
                    err="Cannot do operation on non compatible types!";
                    return;
                }
                front=cpx;
            }else{
                success=cutoff;
                err="Cannot do operation on non compatible types!";
                return;
            }
        }

        switch(op){
            case '+':
                val=front+back;
                break;
            case '-':
                val=front-back;
                break;
            case '*':
            case '(':
            case ')':
                val=front*back;
                break;
            case '/':
                if(back.is_zero()){
                    success=cutoff;
                    err="Cannot divide zero expressions";
                    return;
                }
                if(!back.is_field()){
                    R one=back.promote_one();
                    back=R{new Fraction{back,one}};
                }
                val=front/back;
                break;
        }
    }else{
        R valin;
        //Either unary with bracket or unary without bracket
        if(input.at(unary_cutoff)=='('){
            parse_recurse(input, err, valin, success, unary_cutoff+1, end_index-1, poly_char);
        }else{
            int poly_idx=-1;
            if(poly_char!=-1){
                for(int i=unary_cutoff;i<end_index;i++){
                    if(input.at(i)==poly_char){
                        poly_idx=i;
                        break;
                    }
                }
            }
            int poly_deg=-1;
            if(poly_idx!=-1){
                if(poly_idx==end_index-1){
                    poly_deg=1;
                }else{
                    string degree=input.substr(poly_idx+1, end_index-poly_idx);
                    try{
                        poly_deg=stoi(degree);
                    }catch(...){
                        success=poly_idx;
                        err="Invalid integer.";
                        return;
                    }
                }
                if(poly_deg<1){
                    success=poly_idx;
                    err="Degree of polynomial must be >=1. Do not write polynomial charater for constant term";
                    return;
                }
                R coeff;
                if(poly_idx==unary_cutoff){
                    coeff=R{new Fraction{ R{new Long{1}}, R{new Long{1}} }}; //Assuming long type. To specify double, type 1.0
                }else{
                    simple_parse_cpx(input, err, coeff, success, unary_cutoff, poly_idx);
                    if(success!=-1){
                        return;
                    }
                }
                if(!coeff.is_field()){
                    R one=coeff.promote_one();
                    coeff=R{new Fraction{ coeff, one}};
                }
                R* coeffs=new R[poly_deg+1];
                coeffs[poly_deg]=coeff;

                valin=R{new Polynomial{coeffs, poly_deg+1}};

                delete[] coeffs;
            }else{
                simple_parse_cpx(input, err, valin, success, unary_cutoff, end_index);
                if(success!=-1){
                    return;
                }
            }
        }

        if(negate){
            val= -valin;
        }else{
            val= valin;
        }
    }
}

void parse_expression(const string& input,string& err, R& val, int& success){
    success=-1;
    char poly_char=-1;
    ensure_basic_char(input, poly_char, err, success);
    if(success!=-1){
        return;
    }
    parse_recurse(input,err,val,success,0,input.length(),poly_char);
}

