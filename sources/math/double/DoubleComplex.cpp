#include "math/double/DoubleComplex.h"
#include <sstream>
#include <iomanip>

int DoubleComplex::PRECISION=3; //3dp precision

DoubleComplex::DoubleComplex(const double& re, const double& im) : Field(RingType::DOUBLE), re(re), im(im){
    type->set_sub_type_no_copy(new NestedRingType{RingType::COMPLEXIFY});
}

const Ring* DoubleComplex::addImpl(const Ring* r) const{
    const DoubleComplex* d=dynamic_cast<const DoubleComplex*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new DoubleComplex{re+(d->re), im+(d->im)};
}

const Ring* DoubleComplex::multImpl(const Ring* r) const{
    const DoubleComplex* cpx=dynamic_cast<const DoubleComplex*>(r);
#if DEBUG_MODE
    if(cpx==nullptr){
        throw "invalid cast!";
    }
#endif
    const double &a=re, &b=im, &c=(cpx->re), &d=(cpx->im); //just for convenience.

    double re_new=a*c-b*d;
    double im_new=a*d+b*c;
    return new DoubleComplex{re_new, im_new};
}

const Ring* DoubleComplex::minusImpl(const Ring* r) const{
    const DoubleComplex* d=dynamic_cast<const DoubleComplex*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new DoubleComplex{re-(d->re), im-(d->im)};
}

const Field* DoubleComplex::divImpl(const Ring* r) const{
    const DoubleComplex* cpx=dynamic_cast<const DoubleComplex*>(r);
#if DEBUG_MODE
    if(cpx==nullptr){
        throw "invalid cast!";
    }
#endif
    if(cpx->is_zero()){
        throw "Divide by zero!";
    }
    const double &a=re, &b=im, &c=(cpx->re), &d=(cpx->im); //just for convenience.
    const double norm=c*c+d*d;
    return new DoubleComplex{(a*c+b*d)/norm, (b*c-a*d)/norm};
}

const Field* DoubleComplex::invert() const{
    const double &c=re, &d=im;
    const double norm=c*c+d*d;
    return new DoubleComplex{(d)/norm,(-c)/norm};
}

const DoubleComplex* DoubleComplex::negate() const{
    return new DoubleComplex{-re,-im};
}

const DoubleComplex* DoubleComplex::copy() const{
    return new DoubleComplex{re,im};
}

bool DoubleComplex::equalsImpl(const Ring* other) const{
    // short circuit for the second condition, or else the cast would be invalid.
    const DoubleComplex* d=dynamic_cast<const DoubleComplex*>(other);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return d->re==re && d->im==im;
}

string DoubleComplex::to_string() const{
    ostringstream str;
    str << setprecision(PRECISION);
    str << re;

    if(im>0){
        str << '+' << im << "i";
    }else if(im!=0){
        str << im << "i";
    }
    return str.str();
}

string DoubleComplex::to_signed_string() const{
    ostringstream str;
    str << setprecision(PRECISION);

    if(re>0){
        str << '+' << re;
    }else if(re!=0){
        str << re;
    }

    if(im>0){
        str << '+' << im << "i";
    }else if(im!=0){
        str << im << "i";
    }
    return str.str();
}

string DoubleComplex::to_latex() const{
    return to_signed_latex();
}

string DoubleComplex::to_signed_latex() const{
    return to_signed_string();
}

/**
 * Only subset is itself and double
*/
const DoubleComplex* DoubleComplex::promote(const Ring* const& r) const{
    if(r->type_shallow==RingType::SPECIAL_ZERO){
        return new DoubleComplex{0.0, 0.0};
    }
    const DoubleComplex* d=dynamic_cast<const DoubleComplex*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return d->copy();
}

const DoubleComplex* DoubleComplex::promote_one() const{
    return new DoubleComplex{1.0,0.0};
}

const Ring* DoubleComplex::complexify() const{
    return copy();
}

bool DoubleComplex::is_one() const{
    return re==1 && im==0;
}

bool DoubleComplex::is_unit() const{
    return re!=0 && im!=0;
}

void DoubleComplex::split_canonical(const Ring*& morph, const Ring*& unit) const{
    morph=new DoubleComplex{re, im};
    unit=new DoubleComplex{1.0, 0.0};
}