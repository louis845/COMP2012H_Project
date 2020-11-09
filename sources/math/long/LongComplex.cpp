#include "math/long/LongComplex.h"
#include "math/long/Long.h"
#include <string>
#include <cmath>

LongComplex::LongComplex(const long long& re, const long long& im) : Ring(RingType::COMPLEXIFY), re(re), im(im){
    type->set_sub_type_no_copy(new NestedRingType{RingType::LONG});
}

const Ring* LongComplex::addImpl(const Ring* r) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(r);
    return new LongComplex{re+cpx->re, im+cpx->im};
}

const Ring* LongComplex::multImpl(const Ring* r) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(r);

    const long long &a=re, &b=im, &c=(cpx->re), &d=(cpx->im); //just for convenience.

    long long re_new=a*c-b*d;
    long long im_new=a*d+b*c;

    return new LongComplex{re_new,im_new};
}

const Ring* LongComplex::minusImpl(const Ring* r) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(r);
    return new LongComplex{re-cpx->re, im-cpx->im};
}

const Ring* LongComplex::divImpl(const Ring* r) const{
    return new LongComplex{val/(dynamic_cast<const LongComplex*>(r)->val)};
}

const Ring* LongComplex::remainderImpl(const Ring* r) const {
    return new LongComplex{val%(dynamic_cast<const LongComplex*>(r)->val)};
}

void LongComplex::quotAndRemainder(const Ring* r, const Ring*& quot, const Ring*& remainder){
    
}

const Ring* LongComplex::invert() const{
    return new LongComplex{1/val};
}

const LongComplex* LongComplex::negate() const{
    return new LongComplex{-re,-im};
}

const LongComplex* LongComplex::copy() const{
    return new LongComplex{re,im};
}

int LongComplex::euclideanFuncCompare(const Ring* other) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(other);
    const long long &a=re, &b=im, &c=(cpx->re), &d=(cpx->im); //just for convenience.

    long long otherval=c*c+d*d;

    long long thisval=a*a+b*b;
    return thisval==otherval? 0 : ( thisval<otherval? -1 : 1 );
}

bool LongComplex::equalsImpl(const Ring* other) const{
    const LongComplex* cpx=dynamic_cast<const LongComplex*>(other);
    const long long &a=re, &b=im, &c=(cpx->re), &d=(cpx->im); //just for convenience.

    return a==c && b==d;
}

string LongComplex::to_string() const{
    if(re==0 && im==0){
        return "0";
    }
    if(re!=0 && im!=0){
        
    }
    if(re!=0){

    }
    
    return std::to_string(val);
}

string LongComplex::to_signed_string() const{
    if(val>=0){
        return "+"+std::to_string(val);
    }
    return std::to_string(val);
}

string LongComplex::to_latex() const{
    return std::to_string(val);
}

string LongComplex::to_signed_latex() const{
    if(val>=0){
        return "+"+std::to_string(val);
    }
    return std::to_string(val);
}

/**
 * The only subsets are itself, long and zero
*/
const LongComplex* LongComplex::promote(const Ring* const& r) const{
    if(r->type_shallow==RingType::SPECIAL_ZERO){
        return new LongComplex{0,0};
    }
    if(r->type_shallow==RingType::LONG){
        const Long* lo=static_cast<const Long*>(r);
        return new LongComplex{lo->val,0};
    }
    return static_cast<const LongComplex*>(r)->copy();
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
        
        morph=new LongComplex{-val};
        unit=new LongComplex{-1};
    }
}