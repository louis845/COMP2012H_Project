#include "math/fraction/Fraction.h"

Fraction::Fraction(R over,R under) : Field(RingType::FRACTION){
#if DEBUG
#endif
}

const Ring* Fraction::addImpl(const Ring* r) const{
    return new Fraction{val+(dynamic_cast<const Fraction*>(r)->val)};
}

const Ring* Fraction::multImpl(const Ring* r) const{
    return new Fraction{val*(dynamic_cast<const Fraction*>(r)->val)};
}

const Ring* Fraction::minusImpl(const Ring* r) const{
    return new Fraction{val-(dynamic_cast<const Fraction*>(r)->val)};
}

const Field* Fraction::divImpl(const Ring* r) const{
    return new Fraction{val/(dynamic_cast<const Fraction*>(r)->val)};
}

const Field* Fraction::invert() const{
    return new Fraction{1/val};
}

const Fraction* Fraction::negate() const{
    return new Fraction{-val};
}

const Fraction* Fraction::copy() const{
    return new Fraction{val};
}

bool Fraction::equalsImpl(const Ring* other) const{
    // short circuit for the second condition, or else the cast would be invalid.
    return (other->type==RingType::SPECIAL_ZERO && val==0) ||
     (other->type!=RingType::SPECIAL_ZERO && val==(static_cast<const Fraction*>(other))->val);
}

string Fraction::to_string() const{
    ostringstream str;
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}

string Fraction::to_signed_string() const{
    ostringstream str;
    if(val>=0){
        str << "+";
    }
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}

string Fraction::to_latex() const{
    
    return str.str();
}

string Fraction::to_signed_latex() const{
    ostringstream str;
    if(val>=0){
        str << "+";
    }
    str << setprecision(PRECISION);
    str << val;
    return str.str();
}