#include "math/double/Double.h"
#include <sstream>
#include <iomanip>

int Double::PRECISION=3; //3dp precision

Double::Double(double val) : Field(RingType::DOUBLE), val(val){}

const Ring* Double::addImpl(const Ring* r) const{
    const Double* d=dynamic_cast<const Double*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new Double{val+(d->val)};
}

const Ring* Double::multImpl(const Ring* r) const{
    const Double* d=dynamic_cast<const Double*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new Double{val*(d->val)};
}

const Ring* Double::minusImpl(const Ring* r) const{
    const Double* d=dynamic_cast<const Double*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new Double{val-d->val};
}

const Field* Double::divImpl(const Ring* r) const{
    const Double* d=dynamic_cast<const Double*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return new Double{val/(d->val)};
}

const Field* Double::invert() const{
    return new Double{1/val};
}

const Double* Double::negate() const{
    return new Double{-val};
}

const Double* Double::copy() const{
    return new Double{val};
}

bool Double::equalsImpl(const Ring* other) const{
    // short circuit for the second condition, or else the cast would be invalid.
    return (other->type_shallow==RingType::SPECIAL_ZERO && val==0) ||
     (other->type_shallow!=RingType::SPECIAL_ZERO && val==(dynamic_cast<const Double*>(other))->val);
}

string Double::to_string() const{
    ostringstream str;
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}

string Double::to_signed_string() const{
    ostringstream str;
    if(val>=0){
        str << "+";
    }
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}

string Double::to_latex() const{
    ostringstream str;
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}

string Double::to_signed_latex() const{
    ostringstream str;
    if(val>=0){
        str << "+";
    }
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}

/**
 * Type is primitive anyway, so only subset is itself
*/
const Double* Double::promote(const Ring* const& r) const{
    if(r->type_shallow==RingType::SPECIAL_ZERO){
        return new Double{0};
    }
    const Double* d=dynamic_cast<const Double*>(r);
#if DEBUG_MODE
    if(d==nullptr){
        throw "invalid cast!";
    }
#endif
    return d->copy();
}

const Double* Double::promote_one() const{
    return new Double{1.0};
}

bool Double::is_one() const{
    return val==1;
}

bool Double::is_unit() const{
    return val!=0;
}

void Double::split_canonical(const Ring*& morph, const Ring*& unit) const{
    morph=new Double{val};
    unit=new Double{1.0};
}