#include "math/long/LongComplex.h"
#include "math/long/Long.h"
#include "math/tools.h"
#include <string>
#include <cmath>

LongComplex::LongComplex(const PRIMITIVE_LONG_TYPE &re, const PRIMITIVE_LONG_TYPE &im) : Ring(RingType::COMPLEXIFY), re(re), im(im){
    type->set_sub_type_no_copy(new NestedRingType{RingType::LONG});
}

const Ring* LongComplex::addImpl(const Ring* r) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(r);
#if DEBUG_MODE
    if(cpx==nullptr){
        throw "invalid cast!";
    }
#endif
    return new LongComplex{re+cpx->re, im+cpx->im};
}

const Ring* LongComplex::multImpl(const Ring* r) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(r);
#if DEBUG_MODE
    if(cpx==nullptr){
        throw "invalid cast!";
    }
#endif

    const PRIMITIVE_LONG_TYPE &a=re, &b=im, &c=(cpx->re), &d=(cpx->im); //just for convenience.

    PRIMITIVE_LONG_TYPE re_new=a*c-b*d;
    PRIMITIVE_LONG_TYPE im_new=a*d+b*c;

    return new LongComplex{re_new,im_new};
}

const Ring* LongComplex::minusImpl(const Ring* r) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(r);
#if DEBUG_MODE
    if(cpx==nullptr){
        throw "invalid cast!";
    }
#endif

    return new LongComplex{re-cpx->re, im-cpx->im};
}

const Ring* LongComplex::divImpl(const Ring* r) const{
    const Ring *quot, *rem;
    quotAndRemainder(r, quot, rem);
    delete rem;
    return quot;
}

const Ring* LongComplex::remainderImpl(const Ring* r) const {
    const Ring *quot, *rem;
    quotAndRemainder(r, quot, rem);
    delete quot;
    return rem;
}

/**
 * Multiplies the complex number (a+bi) by (mulRE+mulIM*i). 
*/
void helper_multiply(PRIMITIVE_LONG_TYPE& a, PRIMITIVE_LONG_TYPE& b, PRIMITIVE_LONG_TYPE const& mulRE, PRIMITIVE_LONG_TYPE const& mulIM){
    PRIMITIVE_LONG_TYPE resRE=a*mulRE-b*mulIM;
    PRIMITIVE_LONG_TYPE resIM=a*mulIM+b*mulRE;
    a=resRE;
    b=resIM;
}

void LongComplex::quotAndRemainder(const Ring* r, const Ring*& quot, const Ring*& remainder) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(r);
#if DEBUG_MODE
    if(cpx==nullptr){
        throw "invalid cast!";
    }
#endif

    PRIMITIVE_LONG_TYPE a=re, b=im, c=(cpx->re), d=(cpx->im); //just for convenience, (a+bi)/(c+di)

    if(c==0 && d==0){
        throw "Divide by zero!";
    }

    PRIMITIVE_LONG_TYPE c_factor=gcd(gcd(gcd(a,b),c),d); //Taking common factor of all

    a=a/c_factor;
    b=b/c_factor;
    c=c/c_factor;
    d=d/c_factor;    //Rescaling, so that it is harder to overflow

    PRIMITIVE_LONG_TYPE mulRE=c;
    PRIMITIVE_LONG_TYPE mulIM=-d; //Conjugate

    helper_multiply(a,b,mulRE,mulIM);
    helper_multiply(c,d,mulRE,mulIM);

    PRIMITIVE_LONG_TYPE reQuot=a/c, reRem=a%c;
    PRIMITIVE_LONG_TYPE imQuot=b/c, imRem=b%c;

    if(reRem>(c/2)){
        ++reQuot;
    }else if(reRem<(-(c/2))){
        --reQuot;
    }

    if(imRem>(c/2)){
        ++imQuot;
    }else if(imRem<(-(c/2))){
        --imQuot;
    }

    a=cpx->re; b=cpx->im;

    helper_multiply(a,b,reQuot,imQuot);

    reRem=re-a;
    imRem=im-b;

    quot=new LongComplex{reQuot,imQuot};
    remainder=new LongComplex{reRem,imRem};
}

const Ring* LongComplex::invert() const{

    PRIMITIVE_LONG_TYPE a=1, b=0, c=re, d=im; //just for convenience, (a+bi)/(c+di)

    if(c==0 && d==0){
        throw "Divide by zero!";
    }

    PRIMITIVE_LONG_TYPE mulRE=c;
    PRIMITIVE_LONG_TYPE mulIM=-d; //Conjugate

    helper_multiply(a,b,mulRE,mulIM);
    helper_multiply(c,d,mulRE,mulIM);

    PRIMITIVE_LONG_TYPE reQuot=a/c, reRem=a%c;
    PRIMITIVE_LONG_TYPE imQuot=b/c, imRem=b%c;

    if(reRem>(c/2)){
        ++reQuot;
    }else if(reRem<(-(c/2))){
        --reQuot;
    }

    if(imRem>(c/2)){
        ++imQuot;
    }else if(imRem<(-(c/2))){
        --imQuot;
    }

    return new LongComplex{reQuot, imQuot};
}

const LongComplex* LongComplex::negate() const{
    return new LongComplex{-re,-im};
}

const LongComplex* LongComplex::copy() const{
    return new LongComplex{re,im};
}

int LongComplex::euclideanFuncCompare(const Ring* other) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(other);
#if DEBUG_MODE
    if(cpx==nullptr){
        throw "invalid cast!";
    }
#endif

    const PRIMITIVE_LONG_TYPE &a=re, &b=im, &c=(cpx->re), &d=(cpx->im); //just for convenience.

    PRIMITIVE_LONG_TYPE otherval=c*c+d*d;

    PRIMITIVE_LONG_TYPE thisval=a*a+b*b;
    return thisval==otherval? 0 : ( thisval<otherval? -1 : 1 );
}

bool LongComplex::equalsImpl(const Ring* other) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(other);
#if DEBUG_MODE
    if(cpx==nullptr){
        throw "invalid cast!";
    }
#endif
    const PRIMITIVE_LONG_TYPE &a=re, &b=im, &c=(cpx->re), &d=(cpx->im); //just for convenience.

    return a==c && b==d;
}

string to_string_signed(const PRIMITIVE_LONG_TYPE& val){
    if(val>=0){
        return "+"+CONVERT_TO_STRING(val);
    }
    return CONVERT_TO_STRING(val);
}

string LongComplex::to_string() const{
    if(re==0 && im==0){
        return "0";
    }
    if(re!=0 && im!=0){
        //Non-zero real and imaginary
        if(im==1){
            return CONVERT_TO_STRING(re)+"+i";
        }else if(im==-1){
            return CONVERT_TO_STRING(re)+"-i";
        }
        return CONVERT_TO_STRING(re)+to_string_signed(im)+"i";
    }
    if(re!=0){
        //Non-zero real, zero imaginary
        return CONVERT_TO_STRING(re);
    }

    if(im==1){
        return "i";
    }else if(im==-1){
        return "-i";
    }

    return CONVERT_TO_STRING(im)+"i";
}

string LongComplex::to_signed_string() const{
    if(re==0 && im==0){
        return "+0";
    }
    if(re!=0 && im!=0){
        //Non-zero real and imaginary
        if(im==1){
            return to_string_signed(re)+"+i";
        }else if(im==-1){
            return to_string_signed(re)+"-i";
        }
        return to_string_signed(re)+to_string_signed(im)+"i";
    }
    if(re!=0){
        //Non-zero real, zero imaginary
        return to_string_signed(re);
    }

    if(im==1){
        return "+i";
    }else if(im==-1){
        return "-i";
    }

    return to_string_signed(im)+"i";
}

string LongComplex::to_latex() const{
    return to_string();
}

string LongComplex::to_signed_latex() const{
    return to_signed_string();
}

bool LongComplex::needs_bracket() const{
    return re!=0 && im!=0;
}

bool LongComplex::needs_bracket_latex() const{
    return re!=0 && im!=0;
}

/**
 * The only subsets are itself, long and zero
*/
const LongComplex* LongComplex::promote(const Ring* const& r) const{
    if(r->type_shallow==RingType::SPECIAL_ZERO){
        return new LongComplex{0,0};
    }
    if(r->type_shallow==RingType::LONG){
        const Long* lo=dynamic_cast<const Long*>(r);
#if DEBUG_MODE
        if(lo==nullptr){
            throw "invalid cast!";
        }
#endif
        return new LongComplex{lo->val,0};
    }
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(r);
#if DEBUG_MODE
    if(cpx==nullptr){
        throw "invalid cast!";
    }
#endif
    return cpx->copy();
}

const LongComplex* LongComplex::promote_one() const{
    return new LongComplex{1,0};
}

const Ring* LongComplex::complexify() const{
    return copy();
}

bool LongComplex::is_one() const{
    return re==1 && im==0;
}

bool LongComplex::is_unit() const{
    return (re==1 && im==0)
        || (re==-1 && im==0)
        || (re==0 && im==1)
        || (re==0 && im==-1);
}

void LongComplex::split_canonical(const Ring*& morph, const Ring*& unit) const{
    if(re==0 && im==0){
        morph=new LongComplex{0,0};
        unit=new LongComplex{1,0};
    }else{
        if(re>0 && im>=0){
            morph=new LongComplex{re, im};
            unit=new LongComplex{1,0};
        }else if(re<=0 && im>0){
            morph=new LongComplex{im, -re};
            unit=new LongComplex{0,-1};
        }else if(re<0 && im<=0){
            morph=new LongComplex{-re, -im};
            unit=new LongComplex{-1,0};
        }else{
            morph=new LongComplex{-im, re};
            unit=new LongComplex{0,1};
        }
    }
}